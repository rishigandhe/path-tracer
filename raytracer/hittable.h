#pragma once

#include "hit_record.h"
#include "ray.h"

namespace pathtracer {

class Hittable {
public:
    virtual ~Hittable() = default;

    [[nodiscard]] virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const = 0;
};

}  // namespace pathtracer
