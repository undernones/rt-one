//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Transform.h"

#include <geom/AABB.h>

namespace render
{

Transform::Transform(std::shared_ptr<Renderable>& object)
    : Transform(std::move(object))
{
}

Transform::Transform(std::shared_ptr<Renderable>&& object)
    : Renderable(std::shared_ptr<Material>(nullptr))
    , mLocalScene(nullptr)
    , mObject(std::move(object))
{
}

Transform::~Transform()
{
    if (mLocalScene != nullptr) {
        rtcDeleteScene(mLocalScene);
    }
}

bool
Transform::bbox(float t0, float t1, geom::AABB& bbox) const
{
    if (mObject == nullptr) {
        return false;
    }
    return mObject->bbox(t0, t1, bbox);
}

std::vector<unsigned>
Transform::commit(RTCDevice device, RTCScene scene)
{
    if (mObject == nullptr) {
        return std::vector<unsigned>();
    }

    // First register a new scene consisting of just this transform's renderable.
    mLocalScene = rtcDeviceNewScene(device, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
    mObject->commit(device, mLocalScene);
    rtcCommit(mLocalScene);

    // Then register a user geometry so we can intersect against the sub-object while giving
    // ourselves an opportunity to transform the ray and/or interaction properties.
    mGeomId = rtcNewUserGeometry3(scene, RTC_GEOMETRY_STATIC, 1);
    rtcSetUserData(scene, mGeomId, this);
    rtcSetBoundsFunction2(scene, mGeomId, boundsFunc, this);
    rtcSetIntersectFunction(scene, mGeomId, intersectFunc);
    return std::vector<unsigned>( { mGeomId });
}

void
Transform::boundsFunc(void* userPtr,         /*!< pointer to user data */
                      void* geomUserPtr,     /*!< pointer to geometry user data */
                      size_t item,           /*!< item to calculate bounds for */
                      RTCBounds* bounds_o    /*!< returns calculated bounds */)
{
    // Assume we can dereference userPtr
    const auto renderable = static_cast<render::Renderable*>(userPtr);

    auto bbox = geom::AABB();
    if (!renderable->bbox(0, 1, bbox)) {
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
Transform::intersectFunc(void* userPtr,   /*!< pointer to user data */
                         RTCRay& rtcRay,  /*!< ray to intersect */
                         size_t item      /*!< item to intersect */)
{
    // Assume we can dereference userPtr
    const auto transform = static_cast<render::Transform*>(userPtr);

    auto& ray = (Ray&)rtcRay;

    const auto geomID = ray.geomID;
    ray.geomID = RTC_INVALID_GEOMETRY_ID;

    auto tmpRay = transform->preIntersect(ray);
    rtcIntersect(transform->mLocalScene, (RTCRay&)tmpRay);
    ray = transform->postIntersect(tmpRay);

    if (ray.geomID != RTC_INVALID_GEOMETRY_ID) {
        ray.geomID = transform->mGeomId;
        transform->transform(ray);
    } else {
        ray.geomID = geomID;
    }
}

}
