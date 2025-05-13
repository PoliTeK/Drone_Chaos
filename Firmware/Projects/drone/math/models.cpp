#include "models.hpp"
#include "vecmath.hpp"
#include <cmath>

namespace math {
    // Henon

    vec2f Henon::step(vec2f pos) const {
        return {
            1 - a*pos.x() + pos.y(),
            b*pos.x()
        };
    }

    // Chua

    float Chua::chua_diode(float x) const {
        return m1 * x + 0.5f * (m0 - m1) * (fabs(x + 1) - fabs(x - 1));
    }

    vec3f Chua::step(vec<3, float> pos) const {
        return {
            alpha * (pos.y() - pos.x() - chua_diode(pos.x())),
            pos.x() - pos.y() + pos.z(),
            -beta * pos.y()
        };
    }

}