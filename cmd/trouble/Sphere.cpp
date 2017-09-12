//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Sphere.h"

namespace
{

void
boundsFunc(void* userPtr,         /*!< pointer to user data */
           void* geomUserPtr,     /*!< pointer to geometry user data */
           size_t item,           /*!< item to calculate bounds for */
           RTCBounds* bounds_o    /*!< returns calculated bounds */)
{
    // Assume we can dereference userPtr
    const auto sphere = static_cast<Sphere*>(userPtr);

    auto bbox = geom::AABB();
    if (!sphere->bbox(0, 1, bbox)) {
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
intersectFunc(void* userPtr,   /*!< pointer to user data */
              RTCRay& rtcRay,  /*!< ray to intersect */
              size_t item      /*!< item to intersect */)
{
    // Assume we can dereference userPtr
    const auto sphere = static_cast<Sphere*>(userPtr);

    auto& ray = (Ray&)rtcRay;
    if (sphere->hit(ray)) {
        ray.geomID = sphere->geomId();
        ray.primID = static_cast<unsigned>(item);
    }
}

}

Sphere::Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>& material)
    : Sphere(center, radius, std::move(material))
{
}

Sphere::Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>&& material)
    : Hitable(material)
    , mCenter(center)
    , mRadius(radius)
{
}

bool
Sphere::hit(Ray& ray) const
{
    const auto& origin = ray.origin;
    const auto& direction = ray.direction;

    auto oc = origin - mCenter;
    auto a = direction.dot(direction);
    auto b = oc.dot(direction);
    auto c = oc.dot(oc) - mRadius * mRadius;
    auto discriminant = b*b - a*c;

    auto isHit = false;

    if (discriminant > 0) {
        auto q = sqrt(discriminant);
        auto aInv = 1.f / a;
        auto t0 = (-b - q) * aInv;
        auto t1 = (-b + q) * aInv;
        if (ray.tnear < t0 && t0 < ray.tfar) {
            ray.tfar = t0;
            auto hitPoint = ray.pointAt(t0);
            ray.normal = (hitPoint - mCenter) / mRadius;
            ray.material = material().get();
            std::tie(ray.u, ray.v) = uv(hitPoint, ray.time);
            isHit = true;
        } else
        if (ray.tnear < t1 && t1 < ray.tfar) {
            ray.tfar = t1;
            auto hitPoint = ray.pointAt(t1);
            ray.normal = (hitPoint - mCenter) / mRadius;
            ray.material = material().get();
            std::tie(ray.u, ray.v) = uv(hitPoint, ray.time);
            isHit = true;
        }
    }

    return isHit;
}

bool
Sphere::bbox(float t0, float t1, geom::AABB& bbox) const
{
    auto offset = geom::Vec3(mRadius);
    bbox = geom::AABB(mCenter - offset, mCenter + offset);
    return true;
}

std::tuple<float, float>
Sphere::uv(const geom::Vec3 &p, float t) const
{
    auto normalizedPt = (p - mCenter) / mRadius;

    auto phi = atan2f(normalizedPt.z(), normalizedPt.x());
    auto theta = asinf(normalizedPt.y());

    auto u = 1.f - (phi + M_PI) / (2 * M_PI);
    auto v = (theta + M_PI_2) / M_PI;
    return { u, v };
}

std::vector<unsigned>
Sphere::commit(RTCDevice device, RTCScene scene)
{
    mGeomId = rtcNewUserGeometry3(scene, RTC_GEOMETRY_STATIC, 1);
    rtcSetUserData(scene, mGeomId, this);
    rtcSetBoundsFunction2(scene, mGeomId, boundsFunc, this);
    rtcSetIntersectFunction(scene, mGeomId, intersectFunc);
    return std::vector<unsigned>({ mGeomId });
}
