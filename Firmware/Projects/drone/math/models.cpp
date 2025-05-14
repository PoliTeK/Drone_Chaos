#include "models.hpp"
#include "vecmath.hpp"
#include <cmath>

namespace math {
    // Henon

    vec2f Henon::step(vec2f pos) const {
        return {
            1 - a*pos.x()*pos.x() + pos.y(),
            b*pos.x()
        };
    }

    // Chua

    float Chua::chua_diode(float x) const {
        return m1 * x + 0.5f * (m0 - m1) * (fabs(x + 1) - fabs(x - 1));
    }

    vec3f Chua::step(vec3f pos) const {
        return {
            alpha * (pos.y() - pos.x() - chua_diode(pos.x())),
            pos.x() - pos.y() + pos.z(),
            -beta * pos.y()
        };
    }
     
    vec3f Lorentz::step(vec<3, float> pos) const {
        return {
            sigma * (pos.y() - pos.x()),
            pos.x() * (rho - pos.z()) - pos.y(),
            pos.x() * pos.y() - beta * pos.z()
        };
    }
        vec2f Ikeda::step(vec2f pos) const {
            float r2 = sqrt(pos.x()) +sqrt(pos.y());
            float theta = k - p / (1 + r2);
        return {
            1 + u * (pos.x()*cosf(theta) - pos.y()*sinf(theta)),
            u * (pos.x()*sinf(theta) + pos.y()*cosf(theta))
        };
    }
}