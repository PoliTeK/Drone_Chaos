#pragma once

#include <functional>

#include "vecmath.hpp"

namespace math {

    /**
     * Chaotic model virtual class
     * Both discrete and continuous models extend this class, providing
     * an implementation for `step()`.
     * 
     * @tparam T type of the internal state (e.g. `vec3f`)
     */
    template<class T>
    class ChaoticModel {
    public:
        virtual ~ChaoticModel() {};

        /**
         * For discrete models, step() returns the next state.
         * For continuous models, step() returns the derivative of the state
         * vector, which must be integrated (e.g. using RK4 or explicit Euler)
         */
        virtual T step(T pos) const = 0;
    };

    /**
     * Henon oscillator (discrete, 2D)
     * Useful initial conditions: (x0, y0) = (0.1, 0.3)
     */
    class Henon : ChaoticModel<vec2f>
    {
    public:
        float a = 1.14;
        float b = 0.3;

        vec2f step(vec2f step) const override;
    };

    /**
     * Chua oscillator (continuous, 3D)
     * Useful initial conditions (x0, y0, z0) = (0.1, 0, 0)
     */
    class Chua : ChaoticModel<vec3f>
    {
    public:
        float alpha = 18.39f, beta = 39.0f, m0 = -1.143, m1 = -0.714;

        float chua_diode(float x) const;
        vec3f step(vec3f pos) const override;
    };

    /**
     * @brief Runge-Kutta 4 implementation for N-valued float vectors.
     */
    template<size_t N>
    vec<N, float> rk4(vec<N, float> x, std::function<vec<N, float>(vec<N, float>)> dx, float dt) {
        // Runge-Kutta coefficients
        vec<N, float> k1, k2, k3, k4;
        k1 = dx(x);
        k2 = dx(x + dt * k1 / 2.0f);
        k3 = dx(x + dt * k2 / 2.0f);
        k4 = dx(x + dt * k3);

        return x + dt * (k1 + 2.0f * k2 + 2.0f * k3 + k4) / 6.0f;
    }
     class Lorentz : ChaoticModel<vec3f>
    {
    public:
    float rho = 10;
    float sigma = 10;
    float beta = 8/3;

    vec3f step(vec3f step) const override;
    };
        class Ikeda : ChaoticModel<vec2f>
    {
    public:
        float u = 0.9;
        float k = 0.4;
        float p = 6.0;

        vec2f step(vec2f step) const override;
    };
}