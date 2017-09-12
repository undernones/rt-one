//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_SPHERE_H
#define TROUBLE_SPHERE_H

#include <tuple>
#include <geom/AABB.h>
#include <geom/Vec3.h>

#include "Ray.h"

struct Sphere
{
public:
    Sphere(const geom::Vec3& center, float radius)
        : center(center)
        , radius(radius)
        , geomID(RTC_INVALID_GEOMETRY_ID)
    {}

    bool hit(Ray& ray) const;
    bool bbox(float t0, float t1, geom::AABB& bbox) const;
    std::tuple<float, float> uv(const geom::Vec3& p, float t) const;

    unsigned commit(RTCDevice device, RTCScene scene);

    geom::Vec3 center;
    float radius;
    unsigned geomID;
};

#endif // TROUBLE_SPHERE_H
