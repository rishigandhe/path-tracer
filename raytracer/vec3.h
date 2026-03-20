#pragma once

#include <cassert>
#include <cmath>

namespace pathtracer {

struct Vec3 {
public:
    double xCoord;
    double yCoord;
    double zCoord;

    constexpr Vec3() : xCoord(0.0), yCoord(0.0), zCoord(0.0) {}

    constexpr Vec3(const double x, const double y, const double z)
        : xCoord(x), yCoord(y), zCoord(z) {}

    [[nodiscard]] Vec3 operator-() const {
        return Vec3(-xCoord, -yCoord, -zCoord);
    }

    [[nodiscard]] Vec3 operator*(const Vec3& v) const {
        return Vec3(xCoord * v.xCoord, yCoord * v.yCoord, zCoord * v.zCoord);
    }

    [[nodiscard]] Vec3 operator*(const double s) const {
        return Vec3(xCoord * s, yCoord * s, zCoord * s);
    }

    [[nodiscard]] Vec3 operator+(const Vec3& v) const {
        return Vec3(xCoord + v.xCoord, yCoord + v.yCoord, zCoord + v.zCoord);
    }

    [[nodiscard]] Vec3 operator+(const double s) const {
        return Vec3(xCoord + s, yCoord + s, zCoord + s);
    }

    [[nodiscard]] Vec3 operator/(const Vec3& v) const {
        return Vec3(xCoord / v.xCoord, yCoord / v.yCoord, zCoord / v.zCoord);
    }

    [[nodiscard]] Vec3 operator/(double s) const {
        return Vec3(xCoord / s, yCoord / s, zCoord / s);
    }

    [[nodiscard]] Vec3 operator-(const Vec3& v) const {
        return Vec3(xCoord - v.xCoord, yCoord - v.yCoord, zCoord - v.zCoord);
    }

    [[nodiscard]] constexpr double lengthSquared() const {
        return xCoord * xCoord + yCoord * yCoord + zCoord * zCoord;
    }

    [[nodiscard]] double length() const {
        return std::sqrt(lengthSquared());
    }

    [[nodiscard]] Vec3 normalize() const {
        const double len = this->length();
        assert(len > 0.0 && "normalize: zero-length vector");
        return *this / len;
    }

    [[nodiscard]] constexpr double dot(const Vec3& w) const {
        return xCoord * w.xCoord + yCoord * w.yCoord + zCoord * w.zCoord;
    }

    [[nodiscard]] Vec3 cross(const Vec3& w) const {
        return Vec3(yCoord * w.zCoord - zCoord * w.yCoord,
                    zCoord * w.xCoord - xCoord * w.zCoord,
                    xCoord * w.yCoord - yCoord * w.xCoord);
    }
};

[[nodiscard]] inline Vec3 operator*(const double s, const Vec3& v) {
    return v * s;
}

}  // namespace pathtracer
