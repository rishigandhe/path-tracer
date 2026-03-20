#pragma once

#include "ray.h"

namespace pathtracer {

constexpr double kViewportEdge = 2.0;
constexpr double kViewportMin = -1.0;

struct Viewport {
    Vec3 origin;
    Vec3 lowerLeft;
    Vec3 horizontal;
    Vec3 vertical;
};

[[nodiscard]] constexpr Viewport makeDefaultViewport() {
    return Viewport{
        .origin = Vec3(0, 0, 0),
        .lowerLeft = Vec3(kViewportMin, kViewportMin, kViewportMin),
        .horizontal = Vec3(kViewportEdge, 0, 0),
        .vertical = Vec3(0, kViewportEdge, 0),
    };
}

[[nodiscard]] Ray getRayForPixel(const Viewport& viewport, const int x, const int y, const int width, const int height) {
    const double u = static_cast<double>(x) / static_cast<double>(width - 1);
    const double v = static_cast<double>(y) / static_cast<double>(height - 1);
    const Vec3 point = viewport.lowerLeft + viewport.horizontal * u + viewport.vertical * v;
    return Ray(viewport.origin, (point - viewport.origin).normalize());
}

}  // namespace pathtracer
l