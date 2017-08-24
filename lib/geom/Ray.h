#ifndef GEOM_RAY_H
#define GEOM_RAY_H

#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

#include <geom/Vec3.h>

namespace geom
{

Vec3 pointAlongRay(const float origin[3], const float direction[3], float t);
Vec3 pointAlongRay(const Vec3& origin, const Vec3& direction, float t);
RTCRay newRay(const Vec3& origin, const Vec3& direction, float tnear = 0.f);

}

#endif // GEOM_RAY_H
