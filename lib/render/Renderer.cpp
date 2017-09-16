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
    auto unitDir = ray.direction.normalized();
    auto t = 0.5 * (unitDir.y() + 1.0);
    auto result = (1.0 - t) * geom::Vec3(1, 1, 1) + t * geom::Vec3(0.5, 0.7, 1);
    return scene.bgIntensity() * result;
}

}
