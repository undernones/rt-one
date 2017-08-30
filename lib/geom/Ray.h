//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_RAY_H
#define GEOM_RAY_H

#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

#include <geom/Vec3.h>

namespace geom
{

Vec3 pointAlongRay(const Vec3& origin, const Vec3& direction, float t);
RTCRay newRay(const Vec3& origin, const Vec3& direction, float time, float tnear = 0.f);

}

#endif // GEOM_RAY_H
