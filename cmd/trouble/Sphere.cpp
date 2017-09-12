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
        ray.geomID = sphere->geomID;
        ray.primID = static_cast<unsigned>(item);
    }
}

}

bool
Sphere::hit(Ray& ray) const
{
    const auto& origin = ray.origin;
    const auto& direction = ray.direction;

    auto oc = origin - center;
    auto a = direction.dot(direction);
    auto b = oc.dot(direction);
    auto c = oc.dot(oc) - radius * radius;
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
            ray.normal = (hitPoint - center) / radius;
//            ray.material = material().get();
            std::tie(ray.u, ray.v) = uv(hitPoint, ray.time);
            isHit = true;
        } else
        if (ray.tnear < t1 && t1 < ray.tfar) {
            ray.tfar = t1;
            auto hitPoint = ray.pointAt(t1);
            ray.normal = (hitPoint - center) / radius;
//            ray.material = material().get();
            std::tie(ray.u, ray.v) = uv(hitPoint, ray.time);
            isHit = true;
        }
    }

    return isHit;
}

bool
Sphere::bbox(float t0, float t1, geom::AABB& bbox) const
{
    auto offset = geom::Vec3(radius);
    bbox = geom::AABB(center - offset, center + offset);
    return true;
}

std::tuple<float, float>
Sphere::uv(const geom::Vec3 &p, float t) const
{
    auto normalizedPt = (p - center) / radius;

    auto phi = atan2f(normalizedPt.z(), normalizedPt.x());
    auto theta = asinf(normalizedPt.y());

    auto u = 1.f - (phi + M_PI) / (2 * M_PI);
    auto v = (theta + M_PI_2) / M_PI;
    return { u, v };
}

unsigned
Sphere::commit(RTCDevice device, RTCScene scene)
{
    geomID = rtcNewUserGeometry3(scene, RTC_GEOMETRY_STATIC, 1);
    rtcSetUserData(scene, geomID, this);
    rtcSetBoundsFunction2(scene, geomID, boundsFunc, this);
    rtcSetIntersectFunction(scene, geomID, intersectFunc);
    return geomID;
}
