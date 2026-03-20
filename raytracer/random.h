#pragma once

#include <random>

#include "vec3.h"

namespace pathtracer {

[[nodiscard]] inline double randomDouble() {
    thread_local std::mt19937 generator{std::random_device{}()};
    thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

[[nodiscard]] inline double randomDouble(const double minValue, const double maxValue) {
    return minValue + (maxValue - minValue) * randomDouble();
}

[[nodiscard]] inline Vec3 randomVec3() {
    return Vec3(randomDouble(), randomDouble(), randomDouble());
}

[[nodiscard]] inline Vec3 randomVec3(const double minValue, const double maxValue) {
    return Vec3(randomDouble(minValue, maxValue), randomDouble(minValue, maxValue), randomDouble(minValue, maxValue));
}

[[nodiscard]] inline Vec3 randomInUnitSphere() {
    while (true) {
        const Vec3 p = randomVec3(-1.0, 1.0);
        if (p.lengthSquared() < 1.0) {
            return p;
        }
    }
}

[[nodiscard]] inline Vec3 randomUnitVector() {
    return randomInUnitSphere().normalize();
}

[[nodiscard]] inline Vec3 randomInHemisphere(const Vec3& normal) {
    const Vec3 inUnitSphere = randomInUnitSphere();
    if (inUnitSphere.dot(normal) > 0.0) {
        return inUnitSphere;
    }
    return -inUnitSphere;
}

[[nodiscard]] inline Vec3 randomInUnitDisk() {
    while (true) {
        const Vec3 p(randomDouble(-1.0, 1.0), randomDouble(-1.0, 1.0), 0.0);
        if (p.lengthSquared() < 1.0) {
            return p;
        }
    }
}

}  // namespace pathtracer
