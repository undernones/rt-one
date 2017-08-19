#include "Ray.h"

namespace geom
{

Vec3
pointAlongRay(float origin[3], float direction[3], float t)
{
    return Vec3(origin[0] + t * direction[0],
                origin[1] + t * direction[1],
                origin[2] + t * direction[2]);
}

RTCRay
newRay(const Vec3& origin, const Vec3& direction)
{
    auto ray = RTCRay();
    ray.org[0] = origin.x();
    ray.org[1] = origin.y();
    ray.org[2] = origin.z();
    ray.dir[0] = direction.x();
    ray.dir[1] = direction.y();
    ray.dir[2] = direction.z();
    ray.tnear = 0.f;
    ray.tfar = MAXFLOAT;
    ray.instID = RTC_INVALID_GEOMETRY_ID;
    ray.geomID = RTC_INVALID_GEOMETRY_ID;
    ray.primID = RTC_INVALID_GEOMETRY_ID;
    ray.mask = 0xFFFFFFFF;
    ray.time = 0.f;
    return ray;
}

}
