//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "FlipNormals.h"

#include <geom/AABB.h>

namespace
{

void
boundsFunc(void* userPtr,         /*!< pointer to user data */
           void* geomUserPtr,     /*!< pointer to geometry user data */
           size_t item,           /*!< item to calculate bounds for */
           RTCBounds* bounds_o    /*!< returns calculated bounds */)
{
    // Assume we can dereference userPtr
    const auto hitable = static_cast<render::Hitable*>(userPtr);

    auto bbox = geom::AABB();
    if (!hitable->bbox(0, 1, bbox)) {
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

}

namespace render
{

FlipNormals::FlipNormals(std::shared_ptr<Hitable>& object)
    : FlipNormals(std::move(object))
{
}

FlipNormals::FlipNormals(std::shared_ptr<Hitable>&& object)
    : Hitable(std::shared_ptr<Material>(nullptr))
    , mLocalScene(nullptr)
    , mObject(std::move(object))
{
}

FlipNormals::~FlipNormals()
{
    if (mLocalScene != nullptr) {
        rtcDeleteScene(mLocalScene);
    }
}

std::vector<unsigned>
FlipNormals::commit(RTCDevice device, RTCScene scene)
{
    if (mObject == nullptr) {
        return std::vector<unsigned>();
    }

    // First register a new scene consisting of just this object's hitable.
    mLocalScene = rtcDeviceNewScene(device, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
    auto subGeomIds = mObject->commit(device, mLocalScene);
    rtcCommit(mLocalScene);

    // Then register a user geometry so we can intersect against the sub-object while giving
    // ourselves an opportunity to flip its normals.
    mGeomId = rtcNewUserGeometry3(scene, RTC_GEOMETRY_STATIC, 1);
    rtcSetUserData(scene, mGeomId, this);
    rtcSetBoundsFunction2(scene, mGeomId, boundsFunc, this);
    rtcSetIntersectFunction(scene, mGeomId, intersectFunc);
    return std::vector<unsigned>( { mGeomId });
}

void
FlipNormals::intersectFunc(void* userPtr,   /*!< pointer to user data */
                           RTCRay& rtcRay,  /*!< ray to intersect */
                           size_t item      /*!< item to intersect */)
{
    // Assume we can dereference userPtr
    const auto flipper = static_cast<render::FlipNormals*>(userPtr);

    const auto geomID = rtcRay.geomID;
    rtcRay.geomID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect(flipper->mLocalScene, rtcRay);

    if (rtcRay.geomID != RTC_INVALID_GEOMETRY_ID) {
        rtcRay.geomID = flipper->mGeomId;
        auto& ray = (render::Ray&)rtcRay;
        ray.normal = -ray.normal;
    } else {
        rtcRay.geomID = geomID;
    }
}

bool
FlipNormals::hit(Ray& ray) const
{
    // Unused
    return false;
}

bool
FlipNormals::bbox(float t0, float t1, geom::AABB& bbox) const
{
    return mObject->bbox(t0, t1, bbox);
}

}
