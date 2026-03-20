#pragma once

#include <algorithm>
#include <cmath>
#include <memory>

#include "hit_record.h"
#include "random.h"
#include "ray.h"

namespace pathtracer {

[[nodiscard]] inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2.0 * v.dot(n) * n;
}

[[nodiscard]] inline Vec3 refract(const Vec3& uv, const Vec3& n, const double etaiOverEtat) {
    const double cosTheta = std::min((-uv).dot(n), 1.0);
    const Vec3 rOutPerpendicular = etaiOverEtat * (uv + cosTheta * n);
    const double parallelLenSq = 1.0 - rOutPerpendicular.lengthSquared();
    if (parallelLenSq < 0.0) {
        return Vec3(0.0, 0.0, 0.0);
    }
    const Vec3 rOutParallel = -std::sqrt(parallelLenSq) * n;
    return rOutPerpendicular + rOutParallel;
}

[[nodiscard]] inline double schlickReflectance(const double cosine, const double refIdx) {
    double r0 = (1.0 - refIdx) / (1.0 + refIdx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * std::pow(1.0 - cosine, 5.0);
}

[[nodiscard]] inline bool nearZero(const Vec3& v) {
    constexpr double kEps = 1e-8;
    return std::fabs(v.xCoord) < kEps && std::fabs(v.yCoord) < kEps && std::fabs(v.zCoord) < kEps;
}

class Material {
public:
    virtual ~Material() = default;

    [[nodiscard]] virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian final : public Material {
public:
    explicit Lambertian(const Vec3& albedo) : albedo_(albedo) {}

    [[nodiscard]] bool scatter(const Ray& /*rayIn*/, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override {
        Vec3 scatterDirection = rec.normal + randomUnitVector();
        if (nearZero(scatterDirection)) {
            scatterDirection = rec.normal;
        }
        scattered = Ray(rec.point, scatterDirection.normalize());
        attenuation = albedo_;
        return true;
    }

private:
    Vec3 albedo_;
};

class Metal final : public Material {
public:
    Metal(const Vec3& albedo, const double fuzz) : albedo_(albedo), fuzz_(fuzz < 1.0 ? fuzz : 1.0) {}

    [[nodiscard]] bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override {
        const Vec3 reflected = reflect(rayIn.direction.normalize(), rec.normal);
        scattered = Ray(rec.point, (reflected + fuzz_ * randomInUnitSphere()).normalize());
        attenuation = albedo_;
        return scattered.direction.dot(rec.normal) > 0.0;
    }

private:
    Vec3 albedo_;
    double fuzz_;
};

class Dielectric final : public Material {
public:
    explicit Dielectric(const double indexOfRefraction) : ir_(indexOfRefraction) {}

    [[nodiscard]] bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override {
        attenuation = Vec3(1.0, 1.0, 1.0);
        const double refractionRatio = rec.front_face ? (1.0 / ir_) : ir_;

        const Vec3 unitDirection = rayIn.direction.normalize();
        const double cosTheta = std::min((-unitDirection).dot(rec.normal), 1.0);
        const double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        const bool cannotRefract = refractionRatio * sinTheta > 1.0;
        Vec3 direction;

        if (cannotRefract || schlickReflectance(cosTheta, refractionRatio) > randomDouble()) {
            direction = reflect(unitDirection, rec.normal);
        } else {
            direction = refract(unitDirection, rec.normal, refractionRatio).normalize();
        }

        scattered = Ray(rec.point, direction);
        return true;
    }

private:
    double ir_;
};

}  // namespace pathtracer
