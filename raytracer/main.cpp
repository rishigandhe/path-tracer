// Path tracer: diffuse + metals + glass, depth of field, AA, gamma. P3 PPM on stdout.
// Usage: ./main > output.ppm

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "random.h"
#include "sphere.h"

namespace pathtracer {

// Tune quality vs. speed (higher samples = smoother, slower).
constexpr int kImageWidth = 480;
constexpr int kImageHeight = 270;
constexpr int kSamplesPerPixel = 24;
constexpr int kMaxDepth = 10;
constexpr int kMinImageDimension = 2;
constexpr int kMaxPpmChannel = 255;

void writePpmHeader(const int width, const int height) {
    std::cout << "P3\n" << width << " " << height << "\n" << kMaxPpmChannel << "\n";
}

constexpr int clampPpmChannel(const int value) {
    return value < 0 ? 0 : (value > kMaxPpmChannel ? kMaxPpmChannel : value);
}

// Expects `linearColor` with components roughly in [0, 1] after averaging samples; applies gamma 2.
void writePixelGamma(const Vec3& linearColor) {
    const double r = std::sqrt(std::clamp(linearColor.xCoord, 0.0, 0.999999));
    const double g = std::sqrt(std::clamp(linearColor.yCoord, 0.0, 0.999999));
    const double b = std::sqrt(std::clamp(linearColor.zCoord, 0.0, 0.999999));
    const int ir = clampPpmChannel(static_cast<int>(r * kMaxPpmChannel));
    const int ig = clampPpmChannel(static_cast<int>(g * kMaxPpmChannel));
    const int ib = clampPpmChannel(static_cast<int>(b * kMaxPpmChannel));
    std::cout << ir << ' ' << ig << ' ' << ib << '\n';
}

[[nodiscard]] Vec3 rayColor(const Ray& ray, const Hittable& world, int depth) {
    if (depth <= 0) {
        return Vec3(0.0, 0.0, 0.0);
    }

    HitRecord rec;
    if (world.hit(ray, 0.001, std::numeric_limits<double>::infinity(), rec)) {
        Ray scattered;
        Vec3 attenuation;
        if (rec.mat_ptr != nullptr && rec.mat_ptr->scatter(ray, rec, attenuation, scattered)) {
            return attenuation * rayColor(scattered, world, depth - 1);
        }
        return Vec3(0.0, 0.0, 0.0);
    }

    const Vec3 unitDirection = ray.direction.normalize();
    const double blend = 0.5 * (unitDirection.yCoord + 1.0);
    static const Vec3 white(1.0, 1.0, 1.0);
    static const Vec3 blue(0.5, 0.7, 1.0);
    return white * (1.0 - blend) + blue * blend;
}

[[nodiscard]] HittableList buildScene() {
    HittableList world;

    const auto groundMat = std::make_shared<Lambertian>(Vec3(0.48, 0.53, 0.42));
    const auto matteRed = std::make_shared<Lambertian>(Vec3(0.85, 0.22, 0.18));
    const auto matteGold = std::make_shared<Lambertian>(Vec3(0.9, 0.75, 0.25));
    const auto metalSilver = std::make_shared<Metal>(Vec3(0.92, 0.94, 0.96), 0.08);
    const auto metalCopper = std::make_shared<Metal>(Vec3(0.95, 0.55, 0.32), 0.35);
    const auto glass = std::make_shared<Dielectric>(1.5);

    world.add(std::make_shared<Sphere>(Vec3(0.0, -100.5, -1.0), 100.0, groundMat));

    world.add(std::make_shared<Sphere>(Vec3(0.0, 0.0, -1.0), 0.5, matteRed));
    world.add(std::make_shared<Sphere>(Vec3(-1.15, 0.05, -1.05), 0.42, metalSilver));
    world.add(std::make_shared<Sphere>(Vec3(1.2, -0.05, -1.15), 0.48, glass));
    world.add(std::make_shared<Sphere>(Vec3(1.2, -0.05, -1.15), -0.44, glass));
    world.add(std::make_shared<Sphere>(Vec3(-0.35, -0.25, -0.55), 0.18, matteGold));
    world.add(std::make_shared<Sphere>(Vec3(0.55, -0.35, -0.45), 0.22, metalCopper));

    return world;
}

void render(const int width, const int height, const Hittable& world, const Camera& camera) {
    if (width < kMinImageDimension || height < kMinImageDimension) {
        std::cerr << "render: width and height must be >= " << kMinImageDimension << "\n";
        return;
    }

    writePpmHeader(width, height);

    const double invSamples = 1.0 / static_cast<double>(kSamplesPerPixel);

    for (int j = height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << (j + 1) << "  " << std::flush;
        for (int i = 0; i < width; ++i) {
            Vec3 pixel(0.0, 0.0, 0.0);
            for (int s = 0; s < kSamplesPerPixel; ++s) {
                const double u = (static_cast<double>(i) + randomDouble()) / static_cast<double>(width - 1);
                const double v = (static_cast<double>(j) + randomDouble()) / static_cast<double>(height - 1);
                const Ray r = camera.rayForPixel(u, v);
                pixel = pixel + rayColor(r, world, kMaxDepth);
            }
            writePixelGamma(pixel * invSamples);
        }
    }
    std::cerr << "\rDone.                    \n";
}

}  // namespace pathtracer

int main() {
    using namespace pathtracer;

    const double aspect = static_cast<double>(kImageWidth) / static_cast<double>(kImageHeight);
    const Vec3 lookfrom(0.0, 0.35, 0.35);
    const Vec3 lookat(0.0, 0.0, -1.0);
    const Vec3 vup(0.0, 1.0, 0.0);
    constexpr double kVerticalFov = 55.0;
    constexpr double kAperture = 0.08;
    const double focusDist = (lookfrom - lookat).length();

    const Camera camera(lookfrom, lookat, vup, kVerticalFov, aspect, kAperture, focusDist);
    const HittableList world = buildScene();

    render(kImageWidth, kImageHeight, world, camera);
    return 0;
}
