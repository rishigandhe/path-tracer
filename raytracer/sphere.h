#pragma once

#include <cmath>
#include <memory>
#include <utility>

#include "hittable.h"
#include "material.h"

namespace pathtracer {

class Sphere final : public Hittable {
public:
    Vec3 center;
    double radius;
    std::shared_ptr<Material> mat;

    Sphere(Vec3 cen, const double r, std::shared_ptr<Material> m)
        : center(std::move(cen)), radius(r), mat(std::move(m)) {}

    [[nodiscard]] bool hit(const Ray& ray, const double tMin, const double tMax, HitRecord& rec) const override {
        const Vec3 oc = ray.origin - center;
        const double a = ray.direction.lengthSquared();
        const double halfB = oc.dot(ray.direction);
        const double c = oc.lengthSquared() - radius * radius;

        const double discriminant = halfB * halfB - a * c;
        if (discriminant < 0.0) {
            return false;
        }
        const double sqrtD = std::sqrt(discriminant);

        double root = (-halfB - sqrtD) / a;
        if (root <= tMin || tMax <= root) {
            root = (-halfB + sqrtD) / a;
            if (root <= tMin || tMax <= root) {
                return false;
            }
        }

        rec.t = root;
        rec.point = ray.at(rec.t);
        const Vec3 outwardNormal = (rec.point - center) * (1.0 / radius);
        rec.setFaceNormal(ray, outwardNormal);
        rec.mat_ptr = mat;
        return true;
    }
};

}  // namespace pathtracer
