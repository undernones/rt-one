//
// Copyright © 2017 Undernones. All rights reserved.
//

// Self
#include "Renderer.h"

// System
#include <iostream>
#include <string>
#include <sstream>
#include <embree2/rtcore_ray.h>

// One
#include <geom/Utils.h>

#include "EnvironmentMap.h"
#include "Material.h"
#include "Scene.h"

namespace
{

const auto EPSILON = 0.001f;
const auto MAX_DEPTH = 20;

}

namespace render
{

geom::Vec3
Renderer::trace(Ray ray, const Scene& scene, int depth)
{
    ray.tnear = EPSILON;
    rtcIntersect(scene.rtcScene(), (RTCRay&)ray);
    if (ray.geomID != RTC_INVALID_GEOMETRY_ID) {
        const auto t = ray.tfar;
        const auto hitPoint = ray.pointAt(t);

        // Get the material
        const auto& material = ray.material;

        // Check for emissions
        auto result = material->emitted(ray.uv, hitPoint);

        // Scatter
        auto scattered = Ray();
        auto attenuation = geom::Vec3();
        if (depth < MAX_DEPTH && material->scatter(ray, attenuation, scattered)) {
            result += attenuation * trace(scattered, scene, depth+1);
        }
        
        return result;
    }

    const auto& envMap = scene.environmentMap();
    if (envMap != nullptr) {
        return envMap->value(ray.direction);
    } else {
        return geom::Vec3(0.f);
    }
}

geom::Vec3
Renderer::trace(const std::array<int32_t, 8>& valid, Ray8 rays, const Scene& scene, int depth)
{
    for (auto i = 0; i < 8; ++i) {
        rays.tnear[i] = EPSILON;
    }
    rtcIntersect8(valid.data(), scene.rtcScene(), (RTCRay8&)rays);

    auto hitCount = 0;
    auto result = geom::Vec3(0.f);
    for (auto i = 0; i < 8; ++i) {
        if (rays.geomID[i] != RTC_INVALID_GEOMETRY_ID) {
            hitCount++;
            auto ray = rays.ray(i);
            const auto t = ray.tfar;
            const auto hitPoint = ray.pointAt(t);

            // Get the material
            const auto& material = ray.material;

            // Check for emissions
            result += material->emitted(ray.uv, hitPoint);

            // Scatter
            auto scattered = Ray();
            auto attenuation = geom::Vec3();
            if (depth < MAX_DEPTH && material->scatter(ray, attenuation, scattered)) {
                result += attenuation * trace(scattered, scene, depth+1);
            }
        }
    }
    if (hitCount > 0) {
        return result / static_cast<float>(hitCount);
    }

    const auto& envMap = scene.environmentMap();
    if (envMap != nullptr) {
        auto values = envMap->value8(rays.dirx, rays.diry, rays.dirz);
        return (values[0] +
                values[1] +
                values[2] +
                values[3] +
                values[4] +
                values[5] +
                values[6] +
                values[7]) / 8.f;
    }
    return geom::Vec3(0.f);
}

}
