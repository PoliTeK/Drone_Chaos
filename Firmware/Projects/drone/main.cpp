/* Khaos-V */
/* PoliTeK 2026*/

// This file contains code specialized for the final revision of the project's
// layout.
// To prototype new code, use prototype/prototype.cpp

#include "daisy.h"

#include <array>

#include "math/vecmath.hpp"
// Chaotic models
#include "math/models.hpp"
#include "chaos_osc.hpp"

#include "hardware/i2c_utils.hpp"
#include "hardware/digipot.hpp"

using namespace daisy;

/* --- Definitions ------------------------------------------------------------------------------ */

struct KhaosInput {
    std::array<Encoder, 4> encoders;

    enum AdcChannels {
        ADC_CV0 = 0,
        ADC_CV1,
        ADC_NUM_CHANNELS
    };

    /// @brief Initialize ADC channels and GPIOs
    void init();
} input;

struct KhaosOutput {
    /// @brief Buffers for digital model samples
    std::array<uint16_t, 64> buf1, buf2;

    ChaosOsc<3> rossler;

    /// @brief Initialize DAC channels
    void init();
} output;

enum class Result {
    Ok,
    Err
};

void init_outputs();

/// @brief Initializes timers and sets their callbacks
void init_timers();
void input_timer_callback(void *data);
void output_dma_callback(uint16_t **out, size_t size);
// void display_refresh_callback(void *data);

/// @brief Initialized chaotic models with default parameters
void init_chaotic_models();


/* --- Configuration ---------------------------------------------------------------------------- */
constexpr uint32_t input_refresh_rate = 100; // per second
constexpr uint32_t display_refresh_rate = 60; // per second


/* --- Global variables ------------------------------------------------------------------------- */
AdcHandle adc;

/// TIM3: 16-bit timer
TimerHandle input_timer;
/// TIM4: 16-bit timer
TimerHandle display_timer;


/* --- Main code -------------------------------------------------------------------------------- */

int main(void) {
	DaisySeed hw;
	hw.Init();

#ifdef DEBUG
    hw.StartLog(true);
#else
    hw.StartLog();
#endif

    bool good_init = true;

    input.init();
    output.init();
    init_timers();

    I2CHandle digipot_i2c;
    if (digipot::init(digipot_i2c) == I2CHandle::Result::ERR) {
        good_init = false;
    }

    // Tasks:
    // - read input
    // - propagate parameters to chaotic oscillators
    // - output digital oscillator
    // - manage display

    while (true) {
        __WFE(); // wait for event (low power mode)
    }
}

void KhaosInput::init() {
    AdcChannelConfig adc_config[AdcChannels::ADC_NUM_CHANNELS];
    // Control Voltages
    adc_config[AdcChannels::ADC_CV0].InitSingle(seed::A0);
    adc_config[AdcChannels::ADC_CV1].InitSingle(seed::A1);
    adc.Init(adc_config, AdcChannels::ADC_NUM_CHANNELS);

    // Use a dummy pin (same as pin A) as the switch, which we don't use
    encoders[0].Init(seed::D17, seed::D19, seed::D17);
    encoders[1].Init(seed::D18, seed::D20, seed::D17);
    encoders[2].Init(seed::D2, seed::D3, seed::D3);
    encoders[3].Init(seed::D13, seed::D14, seed::D13);
}

void KhaosOutput::init() {
    init_chaotic_models();

    DacHandle dac;
    DacHandle::Config config;
    config.chn = DacHandle::Channel::BOTH;
    config.buff_state = DacHandle::BufferState::ENABLED;
    config.bitdepth = DacHandle::BitDepth::BITS_12;
    config.mode = DacHandle::Mode::DMA;

    dac.Init(config);
    dac.Start(buf1.data(), buf2.data(), buf1.size(), output_dma_callback);
}

void init_chaotic_models() {
    // TODO: ...
}

void init_timers(KhaosInput &inputs) {
    TimerHandle::Config config;

    /* Input management timer */
    config.dir = TimerHandle::Config::CounterDir::UP;
    config.enable_irq = true; // needed for user callback
    config.periph = TimerHandle::Config::Peripheral::TIM_3;
    input_timer.Init(config);
    input_timer.SetCallback(input_timer_callback);
    input_timer.SetPrescaler(3999); // avoids overflow since the timer is 16-bit
    input_timer.SetPeriod(input_timer.GetFreq() / input_refresh_rate);

    /* Output management timer */
    config.dir = TimerHandle::Config::CounterDir::UP;
    config.enable_irq = true; // needed for user callback
    config.periph = TimerHandle::Config::Peripheral::TIM_4;
    display_timer.Init(config);
    display_timer.SetCallback(display_timer_callback);
    display_timer.SetPrescaler(3999); // avoids overflow since the timer is 16-bit
    display_timer.SetPeriod(input_timer.GetFreq() / display_refresh_rate);

    input_timer.Start();
    display_timer.Start();
}

void input_timer_callback(void *data) {
    /* Encoders */
    int enc_increment[4];

    for (size_t i = 0; i < input.encoders.size(); i++) {
        input.encoders[i].Debounce();
        enc_increment[i] = input.encoders[i].Increment();
    }
    
    /* Control Voltages */
    uint16_t cv[2];
    cv[0] = adc.Get(KhaosInput::ADC_CV0);
    cv[1] = adc.Get(KhaosInput::ADC_CV1);

    // TODO: do something with this data...
}

void output_dma_callback(uint16_t **out, size_t size) {
    // TODO: ...
}

void display_timer_callback(void *data) {
    // TODO: ...
}
