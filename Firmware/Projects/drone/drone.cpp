#include "daisy_seed.h"
#include "daisysp.h"
#include "stm32h7xx_hal.h"

#include "math/models.hpp"
#include "digipot/mcp4452.hpp"

using namespace daisy;
using namespace daisysp;

constexpr size_t AUDIO_BLOCK_SIZE = 64;
DcBlock dcblock;
Oscillator osc;

CpuLoadMeter audio_load_meter;

constexpr size_t NUM_MODELS = 3;
math::ContinuousModel<math::vec3f> *models[NUM_MODELS];
math::vec3f states[NUM_MODELS];
volatile size_t which_model = 0;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t block_size);
void init_spi(DaisySeed &hw);
void i2c_scan(DaisySeed &hw, I2CHandle &i2c);
I2CHandle::Result i2c_check_addr(daisy::I2CHandle &i2c_handle, uint8_t addr);

enum AdcChannels {
	ParamPot = 0,
	FreqPot,
	NUM_CHANNELS
};

int main(void) {
	DaisySeed hw;

	hw.Init();
	hw.StartLog();

	// Audio 
	hw.SetAudioBlockSize(AUDIO_BLOCK_SIZE);
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	audio_load_meter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());

	dcblock.Init(hw.AudioSampleRate());

	osc.Init(hw.AudioSampleRate());
	osc.SetFreq(220.f);
	osc.SetAmp(1.0f);

	hw.StartAudio(AudioCallback);

	// ADC
	// AdcChannelConfig adc_config[AdcChannels::NUM_CHANNELS];
	// adc_config[AdcChannels::ParamPot].InitSingle(seed::A0);
	// adc_config[AdcChannels::FreqPot].InitSingle(seed::A1);

	// hw.adc.Init(adc_config, AdcChannels::NUM_CHANNELS);
	// hw.adc.Start();

	// GPIO
	// Switch button;
	// update_rate is unused...
	// button.Init(seed::D15, 0, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_INVERTED, Switch::PULL_UP);

	// Other init stuff
	// TODO: dynamic alloc = bad, but simple enough
	models[0] = new math::Rossler;
	models[1] = new math::Rossler;
	models[2] = new math::Halvorsen;

	for (size_t i = 0; i < NUM_MODELS; i++) {
		models[i]->dt = 0.0001f; // LFO
		states[i] = math::vec3f{1.0, 0.0, 1.0};
	}

	// Main loop
	hw.PrintLine("End system init");
	hw.PrintLine("Selected model: %d", which_model);
	
	// bool button_state = false;

	I2CHandle i2c_handle;
	digipot::init(i2c_handle);
	i2c_scan(hw, i2c_handle);

	size_t counter = 0;

	while(1) {
		// param_pot = hw.adc.GetFloat(AdcChannels::ParamPot);
		// freq_pot = hw.adc.GetFloat(AdcChannels::FreqPot);

		/*
		button.Debounce();
		if (button.RisingEdge()) {
			which_model = (which_model + 1) % NUM_MODELS;
			hw.PrintLine("Selected model: %d", which_model);
		}
		
		hw.SetLed(button.Pressed());
		*/

		// hw.PrintLine("Param: " FLT_FMT3 " -- Freq: " FLT_FMT3, FLT_VAR3(param_pot), FLT_VAR3(freq_pot));

		// float cpu_load = audio_load_meter.GetAvgCpuLoad() * 100.f;
		// hw.PrintLine("CPU load: " FLT_FMT(2) "%", FLT_VAR(2, cpu_load));

		auto result = digipot::MCP4452::set_value(i2c_handle, digipot::MCP4452::Wiper::Wiper0, 256 - counter);

		if (result == I2CHandle::Result::OK) {
			hw.PrintLine("Set digipot value successfully");
		} else {
			hw.PrintLine("Failed to set digipot value");
		}

		result = i2c_check_addr(i2c_handle, digipot::I2C_ADDRESS);
		if (result == I2CHandle::Result::OK) {
			hw.PrintLine("Digipot is responding to I2C address");
		} else {
			hw.PrintLine("Digipot is NOT responding to I2C address");
		}

		counter = (counter + 32) % 256;
		System::Delay(2000);
	}
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t block_size) {
	using namespace math;

	auto& model = *models[which_model];
	auto& state = states[which_model];

	audio_load_meter.OnBlockStart();

	for (size_t i = 0; i < block_size; i++) {
		float val;
		state = model.step(state);

		if (which_model == 0) {
			val = osc.Process() * 0.3f;
		} else {
			val = state.z() / 10.f;
			val = clamp(val, -1.f, 1.f);

			val = osc.Process() * val;
		}

		val = dcblock.Process(val);
		out[0][i] = val;
		out[1][i] = val;
	}

	audio_load_meter.OnBlockEnd();
}

// TODO: refactor? keep? delete?
// potrebbe servire per il display... ma i digipot usano l'i2c
void init_spi(DaisySeed &hw, SpiHandle &spi_handle) {
	SpiHandle::Config spi_conf;

	spi_conf.periph = SpiHandle::Config::Peripheral::SPI_1;
	spi_conf.mode = SpiHandle::Config::Mode::MASTER;
	spi_conf.direction = SpiHandle::Config::Direction::TWO_LINES;
	spi_conf.nss = SpiHandle::Config::NSS::SOFT; // Chip Select managed via software!
	spi_conf.clock_polarity = SpiHandle::Config::ClockPolarity::HIGH;
	// Note: SPI Digipots operate in mode ONE_EDGE
	// TODO: check operation mode for display
	spi_conf.clock_phase = SpiHandle::Config::ClockPhase::ONE_EDGE;
	spi_conf.pin_config.sclk = seed::D8;
	spi_conf.pin_config.miso = seed::D9;
	spi_conf.pin_config.mosi = seed::D10;
	spi_conf.pin_config.nss = seed::D7; // unused (controlled via software)

	spi_handle.Init(spi_conf);
}

inline I2CHandle::Result i2c_check_addr(daisy::I2CHandle &i2c_handle, uint8_t addr)
{
	// provo a fare una trasmissione vuota
	// nota: il timeout è in ms
	return i2c_handle.TransmitBlocking(addr, nullptr, 0, 10);
}

// thx vittorio
void i2c_scan(DaisySeed &hw, I2CHandle &i2c)
{
    for(uint8_t addr = 1; addr < 127; addr++)
    {
        // provo a fare una trasmissione vuota
        I2CHandle::Result res = i2c.TransmitBlocking(addr, nullptr, 0, 10);

        if(res == I2CHandle::Result::OK)
        {
            hw.PrintLine("Found I2C device at 0x%02X\r\n", addr);
        }
    }
}