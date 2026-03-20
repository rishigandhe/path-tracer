#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "hittable.h"

namespace pathtracer {

class HittableList final : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    void add(std::shared_ptr<Hittable> object) {
        objects.push_back(std::move(object));
    }

    [[nodiscard]] bool hit(const Ray& ray, const double tMin, const double tMax, HitRecord& rec) const override {
        HitRecord temp;
        bool hitAnything = false;
        double closest = tMax;

        for (const auto& object : objects) {
            if (object->hit(ray, tMin, closest, temp)) {
                hitAnything = true;
                closest = temp.t;
                rec = temp;
            }
        }
        return hitAnything;
    }
};

}  // namespace pathtracer
