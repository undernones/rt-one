//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Scene.h"

#include <sstream>

// TODO: get rid of these
#include "ConstantTexture.h"
#include "DiffuseLight.h"
#include "Lambertian.h"
#include "PerlinTexture.h"

namespace
{

std::string
EmbreeErrorToString(RTCError error)
{
    switch (error) {
        case RTC_NO_ERROR:
            return "No error";

        case RTC_UNKNOWN_ERROR:
            return "Unknown";

        case RTC_INVALID_ARGUMENT:
            return "Invalid argument";

        case RTC_INVALID_OPERATION:
            return "Invalid operation";

        case RTC_OUT_OF_MEMORY:
            return "Out of memory";

        case RTC_UNSUPPORTED_CPU:
            return "Unsupported CPU";

        case RTC_CANCELLED:
            return "Cancelled";
    }
}

void
errorHandler(void* userPtr, const RTCError code, const char* str)
{
    std::cerr << "*** Error: " << EmbreeErrorToString(code) << std::endl;
}

}

Scene::Scene()
    : mDevice(nullptr)
    , mScene(nullptr)
{
    mDevice = rtcNewDevice(NULL);
    if (mDevice == nullptr) {
        auto error = rtcDeviceGetError(NULL);
        auto stream = std::stringstream();
        stream << "Failed to initialize device: " << EmbreeErrorToString(error);
        throw std::domain_error(stream.str());
    }

    rtcDeviceSetErrorFunction2(mDevice, errorHandler, nullptr);

    mScene = rtcDeviceNewScene(mDevice, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
    if (mScene == nullptr) {
        auto error = rtcDeviceGetError(mDevice);
        auto stream = std::stringstream();
        stream << "Failed to initialize device: " << EmbreeErrorToString(error);
        throw std::domain_error(stream.str());
    }
}

Scene::~Scene()
{
    for (const auto& geomId : mGeomIds) {
        rtcDeleteGeometry(mScene, geomId);
    }
    if (mScene != nullptr) {
        rtcDeleteScene(mScene);
    }
    if (mDevice != nullptr) {
        rtcDeleteDevice(mDevice);
    }
}

void
Scene::commit()
{
    auto texture = std::make_shared<PerlinTexture>(4);
    auto material = std::make_shared<Lambertian>(texture);
    auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(geom::Vec3(4, 4, 4)));

    // Spheres
    sphere1 = std::make_shared<Sphere>(geom::Vec3(0, -1000, 0), 1000, material);
    sphere1->commit(mDevice, mScene);
    sphere2 = std::make_shared<Sphere>(geom::Vec3(0, 2, 0), 2, material);
    sphere2->commit(mDevice, mScene);

    // Rect
    rect = std::make_shared<RectYZ>(3, 1, 3, -3, -1, light);
    rect->commit(mDevice, mScene);

    // Another sphere
    sphere3 = std::make_shared<Sphere>(geom::Vec3(0, 7, 0), 2, light);
    sphere3->commit(mDevice, mScene);

    rtcCommit(mScene);

    auto eye = geom::Vec3(15, 4, 3);
    auto lookAt = geom::Vec3(0, 2.5, 0);
    auto up = geom::Vec3(0, 1, 0);
    auto focusDistance = (eye - lookAt).length();
    auto aperture = 0.1f;
    auto t0 = 0.f;
    auto t1 = 1.f;
    mCamera = Camera(eye, lookAt, up, 30, 600, 300, aperture, focusDistance, t0, t1);
}
