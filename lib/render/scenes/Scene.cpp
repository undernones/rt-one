//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Scene.h"

#include <sstream>
#include <geom/AABB.h>

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
    for (auto& pair : mMaterials) {
        auto geomId = pair.first;
        rtcDeleteGeometry(mScene, geomId);
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
    return mMaterials.at(geomId);
}

void
Scene::commit()
{
    for (auto& sphere : mSpheres) {
        auto geomId = sphere.commit(mScene);
        mMaterials[geomId] = sphere.material();
    }
    rtcCommit(mScene);
}

}