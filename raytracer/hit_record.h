#pragma once

#include <memory>

#include "ray.h"
#include "vec3.h"

namespace pathtracer {

class Material;

struct HitRecord {
    Vec3 point{0.0, 0.0, 0.0};
    Vec3 normal{0.0, 0.0, 0.0};
    std::shared_ptr<Material> mat_ptr;
    double t = 0.0;
    bool front_face = false;

    void setFaceNormal(const Ray& ray, const Vec3& outwardNormal) {
        front_face = ray.direction.dot(outwardNormal) < 0.0;
        normal = front_face ? outwardNormal : -outwardNormal;
    }
};

}  // namespace pathtracer
