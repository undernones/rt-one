//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Scene.h"

#include <sstream>

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

}

namespace render
{

Scene::Scene()
    : mDevice(nullptr)
    , mScene(nullptr)
    , mSphereGeomId(RTC_INVALID_GEOMETRY_ID)
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
}

Scene::~Scene()
{
    if (mSphereGeomId != RTC_INVALID_GEOMETRY_ID) {
        rtcDeleteGeometry(mScene, mSphereGeomId);
    }
    if (mScene != nullptr) {
        rtcDeleteScene(mScene);
    }
    if (mDevice != nullptr) {
        rtcDeleteDevice(mDevice);
    }
}

std::shared_ptr<Material>
Scene::material(unsigned geomId, unsigned primId) const
{
    return mSpheres[primId].material();
}

void
Scene::sphereBoundsFunc(void* userPtr,         /*!< pointer to user data */
                        void* geomUserPtr,     /*!< pointer to geometry user data */
                        size_t item,           /*!< item to calculate bounds for */
                        RTCBounds* bounds_o    /*!< returns calculated bounds */)
{
    // Assume we can dereference userPtr
    const auto scene = static_cast<Scene*>(userPtr);
    const auto& sphere = scene->mSpheres[item];

    auto bbox = geom::AABB();
    if (!sphere.bbox(sphere.t0(), sphere.t1(), bbox)) {
        return;
    }

    auto lower = bbox.min();
    auto upper = bbox.max();

    bounds_o->lower_x = lower.x();
    bounds_o->lower_y = lower.y();
    bounds_o->lower_z = lower.z();

    bounds_o->upper_x = upper.x();
    bounds_o->upper_y = upper.y();
    bounds_o->upper_z = upper.z();
}

void
Scene::RTCSphereIntersectFunc(void* userPtr,   /*!< pointer to user data */
                              RTCRay& ray,     /*!< ray to intersect */
                              size_t item      /*!< item to intersect */)
{
    // Assume we can dereference ptr
    const auto scene = static_cast<Scene*>(userPtr);
    const auto& sphere = scene->mSpheres[item];
    if (sphere.hit(ray)) {
        ray.geomID = scene->mSphereGeomId;
        ray.primID = static_cast<unsigned int>(item);

        auto hitPoint = geom::pointAlongRay(ray.org, ray.dir, ray.tfar);
        std::tie(ray.u, ray.v) = sphere.uv(hitPoint, ray.time);
    }
}

void
Scene::commit()
{
    mSphereGeomId = rtcNewUserGeometry3(mScene, RTC_GEOMETRY_STATIC, mSpheres.size());
    rtcSetUserData(mScene, mSphereGeomId, this);
    rtcSetBoundsFunction2(mScene, mSphereGeomId, sphereBoundsFunc, this);
    rtcSetIntersectFunction(mScene, mSphereGeomId, RTCSphereIntersectFunc);

    rtcCommit(mScene);
}

}
