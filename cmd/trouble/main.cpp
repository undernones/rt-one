//
//  Copyright © 2017 Undernones. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <tbb/tbb.h>
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <embree2/rtcore_ray.h>
#include <geom/AABB.h>
#include <geom/Vec3.h>

#include "Camera.h"
#include "ConstantTexture.h"
#include "DiffuseLight.h"
#include "Lambertian.h"
#include "PerlinTexture.h"
#include "Ray.h"
#include "Rectangle.h"
#include "Scene.h" // TODO: remove
#include "Sphere.h"

const auto EPSILON = 1e-4;
const auto MAX_DEPTH = 20;
const auto BG_INTENSITY = 0.f;

#if DEBUG
#define PARALLEL 0
#else
#define PARALLEL 1
#endif

geom::Vec3
trace(Ray ray, RTCScene scene, int depth)
{
    ray.tnear = EPSILON;
    rtcIntersect(scene, (RTCRay&)ray);
    if (ray.geomID != RTC_INVALID_GEOMETRY_ID) {
        const auto t = ray.tfar;
        const auto hitPoint = ray.pointAt(t);

        // Get the material
        auto material = ray.material;

        // Check for emissions
        auto result = material->emitted(ray.u, ray.v, hitPoint);

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
    return BG_INTENSITY * result;
}

int
main(int argc, const char * argv[])
{
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    auto NX = 600;
    auto NY = 300;
    auto NS = 100;

    auto scene = Scene();
    scene.commit();

    const auto& camera = scene.camera();

    std::cout << "P3\n" << NX << " " << NY << "\n255\n";
    for (auto row = NY - 1; row >= 0; --row) {
        for (auto col = 0; col < NX; ++col) {
            auto color = geom::Vec3(0, 0, 0);
#if PARALLEL
            tbb::parallel_for(int(0), NS, int(1), [&](int sample) {
#else
            for (auto sample = 0; sample < NS; ++sample) {
#endif
                auto s = (col + drand48()) / NX;
                auto t = (row + drand48()) / NY;
                auto ray = camera.getRay(s, t);

                color += trace(ray, scene.root(), 0);
#if PARALLEL
            });
#else
            }
#endif
            color /= NS;
            color = geom::Vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
            auto ir = std::clamp(int(255.99*color[0]), 0, 255);
            auto ig = std::clamp(int(255.99*color[1]), 0, 255);
            auto ib = std::clamp(int(255.99*color[2]), 0, 255);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}
