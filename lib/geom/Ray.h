#ifndef GEOM_RAY_H
#define GEOM_RAY_H

#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

#include <geom/Vec3.h>

namespace geom
{

Vec3 pointAlongRay(float origin[3], float direction[3], float t);
RTCRay newRay(const Vec3& origin, const Vec3& direction);

}

#endif // GEOM_RAY_H
