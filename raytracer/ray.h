#pragma once

#include "vec3.h"

namespace pathtracer {

struct Ray {
    Vec3 origin;
    Vec3 direction;

    constexpr Ray() = default;

    constexpr Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

    [[nodiscard]] Vec3 at(const double t) const {
        return origin + direction * t;
    }
};

}  // namespace pathtracer
