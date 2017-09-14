//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "ConstantMedium.h"

#include <geom/AABB.h>

#include "Isotropic.h"

namespace
{

const auto EPSILON = 1e-4f;

}

namespace render
{

ConstantMedium::ConstantMedium(std::shared_ptr<Renderable>& boundary, float density, std::shared_ptr<Texture>& texture)
    : ConstantMedium(boundary, density, std::move(texture))
{
}

ConstantMedium::ConstantMedium(std::shared_ptr<Renderable>& boundary, float density, std::shared_ptr<Texture>&& texture)
    : ConstantMedium(std::move(boundary), density, std::move(texture))
{
}

ConstantMedium::ConstantMedium(std::shared_ptr<Renderable>&& boundary, float density, std::shared_ptr<Texture>&& texture)
    : Renderable(std::make_shared<Isotropic>(texture))
    , mLocalScene(nullptr)
    , mBoundary(boundary)
    , mDensity(density)
{
}

ConstantMedium::~ConstantMedium()
{
    if (mLocalScene != nullptr) {
        rtcDeleteScene(mLocalScene);
    }
}

bool
ConstantMedium::bbox(float t0, float t1, geom::AABB& bbox) const
{
    return mBoundary->bbox(t0, t1, bbox);
}

std::vector<unsigned>
ConstantMedium::commit(RTCDevice device, RTCScene scene)
{
    if (mBoundary == nullptr) {
        return std::vector<unsigned>();
    }

    // First register a new scene consisting of this medium's boundary.
    mLocalScene = rtcDeviceNewScene(device, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
    mBoundary->commit(device, mLocalScene);
    rtcCommit(mLocalScene);

    // Then register a user geometry so we can intersect against the boundary while giving ourselves
    // an opportunity to probabilistically intersect its interior.
    mGeomId = rtcNewUserGeometry3(scene, RTC_GEOMETRY_STATIC, 1);
    rtcSetUserData(scene, mGeomId, this);
    rtcSetBoundsFunction2(scene, mGeomId, boundsFunc, this);
    rtcSetIntersectFunction(scene, mGeomId, intersectFunc);
    return std::vector<unsigned>( { mGeomId });
}

void
ConstantMedium::boundsFunc(void* userPtr,         /*!< pointer to user data */
                           void* geomUserPtr,     /*!< pointer to geometry user data */
                           size_t item,           /*!< item to calculate bounds for */
                           RTCBounds* bounds_o    /*!< returns calculated bounds */)
{
    // Assume we can dereference userPtr
    const auto renderable = static_cast<Renderable*>(userPtr);

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
ConstantMedium::intersectFunc(void* userPtr,   /*!< pointer to user data */
                              RTCRay& rtcRay,  /*!< ray to intersect */
                              size_t item      /*!< item to intersect */)
{
    // Assume we can dereference userPtr
    const auto medium = static_cast<ConstantMedium*>(userPtr);

    // Find where the ray enters the boundary.
    auto ray1 = rtcRay;
    ray1.tnear = -MAXFLOAT;
    ray1.tfar = MAXFLOAT;
    rtcIntersect(medium->mLocalScene, ray1);
    if (ray1.geomID == RTC_INVALID_GEOMETRY_ID) {
        return;
    }

    // Find where the ray exits the boundary.
    auto ray2 = rtcRay;
    ray2.tnear = ray1.tfar + EPSILON;
    ray2.tfar = MAXFLOAT;
    rtcIntersect(medium->mLocalScene, ray2);
    if (ray2.geomID == RTC_INVALID_GEOMETRY_ID) {
        return;
    }

    // Clamp to our query boundaries.
    auto tnear = fmax(ray1.tfar, rtcRay.tnear);
    auto tfar = fmin(ray2.tfar, rtcRay.tfar);

    if (tnear >= tfar) {
        return;
    }

    auto& ray = (Ray&)rtcRay;
    auto rayLength = ray.direction.length();
    auto distanceInsideBoundary = (tfar - tnear) * rayLength;
    auto hitDistance = -(1 / medium->mDensity) * log(drand48());

    if (hitDistance > distanceInsideBoundary) {
        return;
    }

    ray.tfar = tnear + hitDistance / rayLength;
    ray.material = medium->material().get();
    ray.geomID = medium->mGeomId;
    // No need to set the normal
}

}
