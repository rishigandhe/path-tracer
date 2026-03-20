#pragma once

#include <cmath>

#include "random.h"
#include "ray.h"
#include "vec3.h"

namespace pathtracer {

[[nodiscard]] inline double degreesToRadians(const double degrees) {
    return degrees * 0.017453292519943295;  // pi / 180
}

class Camera {
public:
    Camera(const Vec3& lookfrom,
           const Vec3& lookat,
           const Vec3& vup,
           const double verticalFovDegrees,
           const double aspectRatio,
           const double aperture,
           const double focusDistance)
        : origin_(lookfrom) {
        const double theta = degreesToRadians(verticalFovDegrees);
        const double h = std::tan(theta / 2.0);
        const double viewportHeight = 2.0 * h;
        const double viewportWidth = aspectRatio * viewportHeight;

        w_ = (lookfrom - lookat).normalize();
        u_ = vup.cross(w_).normalize();
        v_ = w_.cross(u_);

        lens_radius_ = aperture / 2.0;

        horizontal_ = focusDistance * viewportWidth * u_;
        vertical_ = focusDistance * viewportHeight * v_;
        lower_left_corner_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 - focusDistance * w_;
    }

    [[nodiscard]] Ray rayForPixel(const double s, const double t) const {
        const Vec3 rd = lens_radius_ * randomInUnitDisk();
        const Vec3 offset = u_ * rd.xCoord + v_ * rd.yCoord;
        return Ray(origin_ + offset, (lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset).normalize());
    }

private:
    Vec3 origin_;
    Vec3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;
    Vec3 u_;
    Vec3 v_;
    Vec3 w_;
    double lens_radius_;
};

}  // namespace pathtracer
