#include "models.hpp"
#include "vecmath.hpp"
#include <cmath>

namespace math {
    // Henon
    vec2f Henon::step(vec2f pos) const {
        float x = pos.x(), y = pos.y();
        return {
            1 - a*x + y,
            b*x,
        };
    }

    // Chua
    vec3f Chua::gradient(vec3f pos) const {
        float x = pos.x(), y = pos.y(), z = pos.z();
        return {
            alpha * (y - x - chua_diode(x)),
            x - y + z,
            -beta * y,
        };
    }
    
    float Chua::chua_diode(float x) const {
        return m1 * x + 0.5f * (m0 - m1) * (fabs(x + 1) - fabs(x - 1));
    }

    // Sprott
    vec3f Sprott::gradient(vec3f pos) const {
        float x = pos.x(), y = pos.y(), z = pos.z();
        return {
            y + a*x*y + x*z,
            1 - b*x*x + y*z,
            x - x*x - y*y,
        };
    }
     
    vec3f Lorentz::gradient(vec<3, float> pos) const {
        return {
            sigma * (pos.y() - pos.x()),
            pos.x() * (rho - pos.z()) - pos.y(),
            pos.x() * pos.y() - beta * pos.z()
        };
    }

    vec2f Ikeda::step(vec2f pos) const {
        float r2 = sqrt(pos.x()) + sqrt(pos.y());
        float theta = k - p / (1 + r2);
        return {
            1 + u * (pos.x()*cosf(theta) - pos.y()*sinf(theta)),
            u * (pos.x()*sinf(theta) + pos.y()*cosf(theta))
        };
    }

    // Rössler
    vec3f Rossler::gradient(vec3f pos) const {
        float x = pos.x(), y = pos.y(), z = pos.z();
        return {
            -y - z,
            x + a*y,
            b + z*(x - c),
        };
    }

    // Halvorsen
    vec3f Halvorsen::gradient(vec3f pos) const {
        float x = pos.x(), y = pos.y(), z = pos.z();
        return {
            -a*x - 4*(y + z) - y*y,
            -a*y - 4*(z + x) - z*z,
            -a*z - 4*(x + y) - x*x
        };
    }
}