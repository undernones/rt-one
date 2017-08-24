#include "Ray.h"

namespace geom
{

Vec3
pointAlongRay(const float origin[3], const float direction[3], float t)
{
    return Vec3(origin[0] + t * direction[0],
                origin[1] + t * direction[1],
                origin[2] + t * direction[2]);
}

Vec3
pointAlongRay(const Vec3& origin, const Vec3& direction, float t)
{
    return origin + t * direction;
}

RTCRay
newRay(const Vec3& origin, const Vec3& direction, float tnear)
{
    auto ray = RTCRay();
    ray.org[0] = origin.x();
    ray.org[1] = origin.y();
    ray.org[2] = origin.z();
    ray.dir[0] = direction.x();
    ray.dir[1] = direction.y();
    ray.dir[2] = direction.z();
    ray.tnear = tnear;
    ray.tfar = MAXFLOAT;
    ray.instID = RTC_INVALID_GEOMETRY_ID;
    ray.geomID = RTC_INVALID_GEOMETRY_ID;
    ray.primID = RTC_INVALID_GEOMETRY_ID;
    ray.mask = 0xFFFFFFFF;
    ray.time = 0.f;
    return ray;
}

}
