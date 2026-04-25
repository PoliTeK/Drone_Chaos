/* Khaos-V */
/* PoliTeK 2026*/

// This file contains code specialized for the final revision of the project's
// layout.
// To prototype new code, use prototype/prototype.cpp

#include <daisy_seed.h>

#include <array>
#include <atomic>
#include <limits>

#include "math/vecmath.hpp"
// Chaotic models
#include "math/chaos_osc.hpp"
#include "math/models.hpp"

#include "hardware/digipot.hpp"
#include "hardware/i2c_utils.hpp"

#include "sync/TripleBuffer.hpp"

using namespace daisy;


/* --- Configuration ---------------------------------------------------------------------------- */
constexpr bool DEBUG = false;

constexpr uint32_t INPUT_SAMPLE_RATE = 100;   // per second
constexpr uint32_t OUTPUT_SAMPLE_RATE = 100;  // per second
constexpr uint32_t DISPLAY_REFRESH_RATE = 30; // per second

/// Number of samples stored in the output buffer
constexpr size_t OUTPUT_BUFFER_SIZE = 128;

/// @brief Sets how many 'ticks' cover the full range.
/// Controls the resolution for encoder-controlled parameters.
constexpr uint16_t ROTARY_ENCODER_RESOLUTION = 32;
constexpr size_t PARAM_RESOLUTION = 1024;


/* --- Definitions ------------------------------------------------------------------------------ */

/** Contains all hardware handles related to inputs.
 *  It is safely shared between interrupt callbacks because it does not contain
 *  actual input data, as long as only one "process" updates it.
 */
struct KhaosInput {
    AdcHandle adc;
    std::array<Encoder, 4> encoders;

    enum AdcChannels { ADC_CV0 = 0, ADC_CV1, ADC_NUM_CHANNELS };

    /// @brief Initialize ADC channels and GPIOs
    void init();
};

/** Contains all hardware handles related to outputs.
 *  It is safely shared between interrupt callbacks because it does not contain
 *  actual output data.
 */
struct KhaosOutput {
    DacHandle dac;
    std::array<GPIO, 3> leds;

    /// @brief Initialize DAC channels
    void init();
};

/** Input data coming from external hardware.
 *  This must be synchronized across interrupts and processed in order to be
 *  used to control chaotic models and other outputs.
 */
struct KhaosInputData {
    std::array<uint16_t, 4> encoder_values;
    std::array<bool, 4> switches;
    std::array<uint16_t, 2> cvs;
};

// TODO: choose models
struct KhaosModelData {
    enum SelectedModel { ROSSLER = 0, NUM_MODELS } selected;
    math::Rossler rossler;
};

/// @brief Initializes timers, WITHOUT starting them or initializing callbacks.
void init_timers();
void input_timer_callback(void *data);
void output_dma_callback(uint16_t **out, size_t size);
void display_refresh_callback(void *data);

// /// @brief Initialized chaotic models with default parameters
// void init_chaotic_models();


/* --- Global variables ------------------------------------------------------------------------- */

/// TIM3: 16-bit timer
TimerHandle input_timer;
/// TIM4: 16-bit timer
TimerHandle display_timer;

KhaosInput input;
KhaosOutput output;
TripleBuffer<KhaosInputData> input_data;
TripleBuffer<KhaosModelData> model_data;
std::array<uint16_t, OUTPUT_BUFFER_SIZE> output_buf[2];


/* --- Main code -------------------------------------------------------------------------------- */

int main(void) {
    DaisySeed hw;
    hw.Init();
    hw.StartLog(DEBUG);

    // no one has access to these yet
    // we must ensure that at most one "process" (i.e. interrupt callback)
    // has access to one of these at any time
    auto in_writer = input_data.get_writer().value();
    auto in_reader = input_data.get_reader().value();
    auto model_writer = model_data.get_writer().value();

    input.init();
    output.init();
    init_timers();

    input_timer.SetCallback(input_timer_callback, &in_writer);
    input_timer.Start();

    display_timer.SetCallback(display_refresh_callback);
    display_timer.Start();

    I2CHandle digipot_i2c;
    if (digipot::init(digipot_i2c) != I2CHandle::Result::OK) {
        hw.PrintLine("[Khaos-V] Could not find the I2C digipot");
        goto bad_init;
    }

    // Tasks:
    // - read input
    // - propagate parameters to chaotic oscillators
    // - output digital oscillator
    // - manage display

    hw.PrintLine("[Khaos-V] System initialized successfully");

    while (true) {
        // Process input data
        if (in_reader.try_swap()) {
            auto in_data = in_reader.data();
            auto m_data = model_writer.data();

            uint16_t params[2];

            for (size_t i = 0; i < 2; i++) {
                int32_t raw_value = static_cast<int32_t>(in_data.cvs[i])
                    + static_cast<int32_t>(in_data.encoder_values[i]);
                
                constexpr uint16_t max_value = std::numeric_limits<uint16_t>::max();

                params[i] = static_cast<float>(math::clamp<int32_t>(raw_value, 0, max_value));
            }

            // TODO: map params to model-specific (float) parameters
            // m_data.translate_params(...);
            model_writer.swap();
        }

        __WFE(); // wait for event (low power mode)
    }

bad_init:
    hw.SetLed(true);
    hw.PrintLine("[Khaos-V] Something went wrong during the initialization");

    output.dac.Stop();
    display_timer.DeInit();
    input_timer.DeInit();
    hw.DeInit();

    while (true) {
        __WFE();
    }
}

void KhaosInput::init() {
    AdcChannelConfig adc_config[AdcChannels::ADC_NUM_CHANNELS];

    // Setup Control Voltages
    adc_config[AdcChannels::ADC_CV0].InitSingle(seed::A0);
    adc_config[AdcChannels::ADC_CV1].InitSingle(seed::A1);
    adc.Init(adc_config, AdcChannels::ADC_NUM_CHANNELS);
    adc.Start();

    encoders[0].Init(seed::D17, seed::D18, seed::D24); // input 1
    encoders[1].Init(seed::D19, seed::D20, seed::D25); // input 2
    encoders[2].Init(seed::D2, seed::D3, seed::D26); // selezione modello: analog1, analog2 o digitale
    encoders[3].Init(seed::D13, seed::D14, seed::D27); // selezione modello: quale digitale?
}

void KhaosOutput::init() {
    // DAC configuration
    DacHandle::Config dac_config;
    dac_config.chn = DacHandle::Channel::BOTH;
    dac_config.buff_state = DacHandle::BufferState::DISABLED;
    dac_config.bitdepth = DacHandle::BitDepth::BITS_12;
    dac_config.mode = DacHandle::Mode::DMA;
    dac_config.target_samplerate = OUTPUT_SAMPLE_RATE;
    dac.Init(dac_config);

    // LED configuration
    GPIO::Config led_config;
    led_config.mode = GPIO::Mode::OUTPUT;

    led_config.pin = seed::D4;
    leds[0].Init(led_config);

    led_config.pin = seed::D5;
    leds[1].Init(led_config);

    led_config.pin = seed::D6;
    leds[2].Init(led_config);
}

KhaosInputData::KhaosInputData() {
    // Initialize each encoder value at half range
    for (size_t i = 0; i < encoder_values.size(); i++) {
        encoder_values[i] = PARAM_RESOLUTION / 2;
    }
}

void init_timers() {
    TimerHandle::Config config;

    /* Input management timer */
    config.dir = TimerHandle::Config::CounterDir::UP;
    config.enable_irq = true; // needed for user callback
    config.periph = TimerHandle::Config::Peripheral::TIM_3;
    input_timer.Init(config);
    input_timer.SetCallback(input_timer_callback);
    input_timer.SetPrescaler(3999); // avoids overflow since the timer is 16-bit
    input_timer.SetPeriod(input_timer.GetFreq() / INPUT_SAMPLE_RATE);

    /* Output management timer */
    config.dir = TimerHandle::Config::CounterDir::UP;
    config.enable_irq = true; // needed for user callback
    config.periph = TimerHandle::Config::Peripheral::TIM_4;
    display_timer.Init(config);
    display_timer.SetCallback(display_refresh_callback);
    display_timer.SetPrescaler(3999); // avoids overflow since the timer is 16-bit
    display_timer.SetPeriod(input_timer.GetFreq() / DISPLAY_REFRESH_RATE);
}

void input_timer_callback(void *data) {
    static KhaosInputData local_data;

    /* Encoders */
    for (size_t i = 0; i < input.encoders.size(); i++) {
        input.encoders[i].Debounce();
        int increment = input.encoders[i].Increment();

        int new_value = (int)local_data.encoder_values[i] +
            increment * (PARAM_RESOLUTION / ROTARY_ENCODER_RESOLUTION);

        constexpr uint16_t max_value = std::numeric_limits<uint16_t>::max();

        // Clamp encoder value between 0 and (2^16 - 1)
        if (new_value < 0) {
            local_data.encoder_values[i] = 0;
        } else if (new_value > static_cast<int>(max_value)) {
            local_data.encoder_values[i] = max_value;
        } else {
            local_data.encoder_values[i] = static_cast<uint16_t>(new_value);
        }

        // Gather switch data
        // TODO: change to FallingEdge; do not save switch data directly,
        // (falling edges may be lost due to how TripleBuffer works),
        // but rather select here which chaotic model to use
        local_data.switches[i] = input.encoders[i].Pressed();
    }

    /* Control Voltages */
    local_data.cvs[0] = input.adc.Get(KhaosInput::ADC_CV0);
    local_data.cvs[1] = input.adc.Get(KhaosInput::ADC_CV1);

    // Synchronize data
    auto& writer = *static_cast<TripleBuffer<KhaosInputData>::Writer *>(data);
    writer.data() = local_data;
    writer.swap();
}

void output_dma_callback(uint16_t **out, size_t size) {
    static ChaosOsc<math::Rossler> rossler(
        math::Rossler{}, math::vec3f{1.0f, 1.0f, 1.0f},
        static_cast<float>(OUTPUT_SAMPLE_RATE), 1.0f);
    // TODO: other models...
    
    static KhaosModelData local_data;

    auto try_read = model_data.get_reader();
    if (try_read) {
        auto& reader = try_read.value();

        // update model parameters with new data if possible
        if (reader.try_swap())
            local_data = reader.data();
    }

    // Generate output samples
    // TODO: ...
}

void display_refresh_callback(void *data) {
    // TODO: ...
}
