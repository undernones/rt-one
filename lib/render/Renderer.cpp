//
// Copyright © 2017 Undernones. All rights reserved.
//

// Self
#include "Renderer.h"

// System
#include <iostream>
#include <string>
#include <sstream>

// One
#include <geom/Ray.h>
#include <geom/Utils.h>

#include "Sphere.h"

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
RTCSphereBoundsFunc(void* userPtr,         /*!< pointer to user data */
                    void* geomUserPtr,     /*!< pointer to geometry user data */
                    size_t item,           /*!< item to calculate bounds for */
                    RTCBounds* bounds_o    /*!< returns calculated bounds */)
{
    // Assume we can dereference geomUserPtr
    const auto& sphere = *(static_cast<render::Sphere*>(userPtr)+item);

    auto lower = sphere.center() - geom::Vec3(sphere.radius());
    auto upper = sphere.center() + geom::Vec3(sphere.radius());

    bounds_o->lower_x = lower.x();
    bounds_o->lower_y = lower.y();
    bounds_o->lower_z = lower.z();

    bounds_o->upper_x = upper.x();
    bounds_o->upper_y = upper.y();
    bounds_o->upper_z = upper.z();
}

void
RTCSphereIntersectFunc(void* ptr,           /*!< pointer to user data */
                       RTCRay& ray,         /*!< ray to intersect */
                       size_t item          /*!< item to intersect */)
{
    // Assume we can dereference ptr
    const auto& sphere = *(static_cast<render::Sphere*>(ptr)+item);
    if (sphere.hit(ray)) {
        ray.geomID = 0; // TODO: this isn't future proof
        ray.primID = static_cast<unsigned int>(item);
    }
}

}

namespace render
{

Renderer::Renderer()
{
    mDevice = rtcNewDevice(NULL);
    if (mDevice == nullptr) {
        auto error = rtcDeviceGetError(NULL);
        auto stream = std::stringstream();
        stream << "Failed to initialize device: " << EmbreeErrorToString(error);
        throw std::domain_error(stream.str());
    }

    mScene = rtcDeviceNewScene(mDevice, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
    if (mScene == nullptr) {
        auto error = rtcDeviceGetError(mDevice);
        auto stream = std::stringstream();
        stream << "Failed to initialize device: " << EmbreeErrorToString(error);
        throw std::domain_error(stream.str());
    }

    mSpheres.emplace_back(geom::Vec3(0, 0, -1), 0.5);
    mSpheres.emplace_back(geom::Vec3(0, -100.5, -1), 100);
    auto geomId = rtcNewUserGeometry3(mScene, RTC_GEOMETRY_STATIC, mSpheres.size());
    rtcSetUserData(mScene, geomId, mSpheres.data());
    rtcSetBoundsFunction2(mScene, geomId, RTCSphereBoundsFunc, mSpheres.data());
    rtcSetIntersectFunction(mScene, geomId, RTCSphereIntersectFunc);

    rtcCommit(mScene);
}

Renderer::~Renderer()
{
    if (mDevice != nullptr) {
        rtcDeleteDevice(mDevice);
    }
    if (mScene != nullptr) {
        rtcDeleteScene(mScene);
    }
}

geom::Vec3
Renderer::color(RTCRay ray)
{
    rtcIntersect(mScene, ray);
    if (ray.geomID != RTC_INVALID_GEOMETRY_ID) {
        const auto t = ray.tfar;
        const auto normal = geom::Vec3(ray.Ng);

        const auto hitPoint = geom::pointAlongRay(ray.org, ray.dir, t);
        const auto target = hitPoint + normal + geom::randomInUnitSphere();
        const auto newRay = geom::newRay(hitPoint, target - hitPoint, 0.001);
        return 0.5 * color(newRay);
    }
    auto unitDir = geom::Vec3(ray.dir).normalized();
    auto t = 0.5 * (unitDir.y() + 1.0);
    return (1.0 - t) * geom::Vec3(1, 1, 1) + t * geom::Vec3(0.5, 0.7, 1);
}

}
