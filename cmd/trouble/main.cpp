//
//  Copyright © 2017 Undernones. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <embree2/rtcore_ray.h>
#include <geom/AABB.h>
#include <geom/Vec3.h>

#include "Camera.h"
#include "ConstantTexture.h"
#include "DiffuseLight.h"
#include "Lambertian.h"
#include "Ray.h"
#include "Rectangle.h"
#include "Sphere.h"

const auto EPSILON = 1e-4;
const auto MAX_DEPTH = 40;
const auto BG_INTENSITY = 0.f;

geom::Vec3
trace(Ray ray, RTCScene scene, int depth)
{
    static auto fallback = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(geom::Vec3(0.5, 0.5, 0.5)));
    ray.tnear = EPSILON;
    rtcIntersect(scene, (RTCRay&)ray);
    if (ray.geomID != RTC_INVALID_GEOMETRY_ID) {
        const auto t = ray.tfar;
        const auto hitPoint = ray.pointAt(t);

        // Get the material
        auto material = ray.material;
        if (material == nullptr) {
            material = fallback.get();
        }

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
    auto NS = 20;

    auto device = rtcNewDevice(NULL);
    auto mainScene = rtcDeviceNewScene(device, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);

    auto texture = std::make_shared<ConstantTexture>(geom::Vec3(1, 0.5, 0));
    auto material = std::make_shared<Lambertian>(texture);
    auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(geom::Vec3(4, 4, 4)));

    // Spheres
    auto sphere1 = Sphere(geom::Vec3(0, -1000, 0), 1000, material);
    sphere1.commit(device, mainScene);
    auto sphere2 = Sphere(geom::Vec3(0, 2, 0), 2, material);
    sphere2.commit(device, mainScene);

    // Rect
    auto rect = RectYZ(3, 1, 3, -3, -1);
    rect.commit(device, mainScene);

    // Another sphere
    auto sphere3 = Sphere(geom::Vec3(0, 7, 0), 2, light);
    sphere3.commit(device, mainScene);

    rtcCommit(mainScene);

    auto eye = geom::Vec3(15, 4, 3);
    auto lookAt = geom::Vec3(0, 2.5, 0);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = (eye - lookAt).length();
    auto aperture = 0.1f;
    auto t0 = 0.f;
    auto t1 = 1.f;
    auto camera = Camera(eye, lookAt, up, 30, NX, NY, aperture, focusDistance, t0, t1);

    std::cout << "P3\n" << NX << " " << NY << "\n255\n";
    for (auto row = NY - 1; row >= 0; --row) {
        for (auto col = 0; col < NX; ++col) {
            auto color = geom::Vec3(0, 0, 0);
            for (auto sample = 0; sample < NS; ++sample) {
                auto s = (col + drand48()) / NX;
                auto t = (row + drand48()) / NY;
                auto ray = camera.getRay(s, t);

                color += trace(ray, mainScene, 0);
            }
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
