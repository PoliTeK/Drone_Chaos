#include "daisy_seed.h"
#include "daisysp.h"

#include "math/models.hpp"
#include "ChaosOsc.hpp"

using namespace daisy;
using namespace daisysp;

constexpr size_t AUDIO_BLOCK_SIZE = 32;

CpuLoadMeter audio_load_meter;
DcBlock audio_dcblock;
volatile static float param_pot = 0.0;
volatile static float freq_pot = 0.0;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t block_size);

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
	hw.StartAudio(AudioCallback);

	audio_load_meter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());

	audio_dcblock.Init(hw.AudioSampleRate());

	// ADC
	AdcChannelConfig adc_config[AdcChannels::NUM_CHANNELS];
	adc_config[AdcChannels::ParamPot].InitSingle(seed::A0);
	adc_config[AdcChannels::FreqPot].InitSingle(seed::A1);

	hw.adc.Init(adc_config, AdcChannels::NUM_CHANNELS);
	hw.adc.Start();

	// Main loop
	hw.PrintLine("End system init");

	while(1) {
		param_pot = hw.adc.GetFloat(AdcChannels::ParamPot);
		freq_pot = hw.adc.GetFloat(AdcChannels::FreqPot);
		// hw.PrintLine("Param: " FLT_FMT3 " -- Freq: " FLT_FMT3, FLT_VAR3(param_pot), FLT_VAR3(freq_pot));

		// float cpu_load = audio_load_meter.GetAvgCpuLoad() * 100.f;
		// hw.PrintLine("CPU load: " FLT_FMT(2) "%", FLT_VAR(2, cpu_load));

		System::Delay(100);
	}
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t block_size) {
	using namespace math;

	static Halvorsen model;
	static vec3f state{1.0, 0.0, 1.0};

	audio_load_meter.OnBlockStart();

	for (size_t i = 0; i < block_size; i++) {
		model.a = 1.4f + param_pot * 1.6f;
		model.dt = 0.005f + freq_pot * 0.01f;

		state = model.step(state);
		float val = state.z();

		val = audio_dcblock.Process(val) / 100.f;
		out[0][i] = val;
		out[1][i] = val;
	}

	audio_load_meter.OnBlockEnd();
}