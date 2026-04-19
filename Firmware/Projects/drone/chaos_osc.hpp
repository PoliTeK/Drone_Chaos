#pragma once

#include "daisy_seed.h"
#include "math/models.hpp"
#include "math/vecmath.hpp"

template<size_t N>
class ChaosOsc {
private:
    /// @brief External sampling frequency
    float sampling_frequency; // Hz
    /// @brief Multiplies the intrinsic frequency of the chaotic model by this value
    float freq_multiplier;
    /// @brief Maximum time step allowed for the discretized model
    float max_dt;
    /// @brief Ratio between desired time step and max_dt
    float dt_overshoot;

    void update_dt() {
        float new_dt = freq_multiplier / sampling_frequency;

        if (new_dt <= max_dt) {
            model.dt = new_dt;
            dt_overshoot = 0;
        } else {
            model.dt = max_dt;
            dt_overshoot = (new_dt / max_dt);
        }
    }

public:
    using Model = math::ContinuousModel<math::vec<N, float>>;
    Model model;
    vec<N, float> state;

    ChaosOsc(Model model, vec<N, float> initial_state, float sampling_frequency, float freq_multiplier, float max_dt = math::DT_DEFAULT)
        : model(model), state(initial_state),
        sampling_frequency(sampling_frequency),
        freq_multiplier(freq_multiplier), max_dt(max_dt)
    {
        update_dt()
    }

    void set_sampling_frequency(float new_sampling_frequency) {
        sampling_frequency = new_sampling_frequency;
        update_dt();
    }

    void set_frequency_multiplier(float new_frequency_multiplier) {
        freq_multiplier = new_frequency_multiplier;
        update_dt();
    }

    void set_max_dt(float new_max_dt) {
        max_dt = new_max_dt;
        update_dt();
    }

    void reset(float new_sampling_frequency, float new_frequency_multiplier, float new_max_dt) {
        sampling_frequency = new_sampling_frequency;
        freq_multiplier = new_frequency_multiplier;
        max_dt = new_max_dt;
        update_dt();
    }
    
    vec<N, float> step() {
        if (dt_overshoot == 0) {
            state = model.step(state);
        } else {
            size_t overshoot = static_cast<size_t>(truncf(dt_overshoot));

            // step 'overshoot' times using max_dt
            for (size_t k = overshoot; k > 0; k--) {
                state = model.step(state);
            }

            // do a single step with the remaining dt
            float remainder_dt = dt_overshoot - overshoot * max_dt;
            model.dt = remainder_dt;
            state = model.step(state);
            model.dt = max_dt;
        }

        return state;
    }
};