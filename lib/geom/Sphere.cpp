//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Sphere.h"

namespace geom
{

Sphere::Sphere()
    : Sphere(Vec3(0, 0, 0), 1.f)
{
}

Sphere::Sphere(const Vec3& center, float radius)
    : mCenter(center)
    , mRadius(radius)
{
}

std::tuple<float, float>
Sphere::uv(const Vec3 &p) const
{
    auto normalizedPt = (p - mCenter) / mRadius;

    auto phi = atan2f(normalizedPt.z(), normalizedPt.x());
    auto theta = asinf(normalizedPt.y());

    auto u = 1.f - (phi + M_PI) / (2 * M_PI);
    auto v = (theta + M_PI_2) / M_PI;
    return { u, v };
}

}
