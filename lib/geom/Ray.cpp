#include "Ray.h"

namespace geom
{

Vec3
pointAlongRay(const Vec3& origin, const Vec3& direction, float t)
{
    return origin + t * direction;
}

RTCRay
newRay(const Vec3& origin, const Vec3& direction, float time, float tnear)
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
    ray.time = time;

    return ray;
}

}
