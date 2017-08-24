//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Utils.h"

#include <stdlib.h>

namespace geom
{

Vec3
randomInUnitSphere()
{
    auto p = Vec3();
    const auto unitSphere = Vec3(1, 1, 1);
    do {
        p = 2 * Vec3(drand48(), drand48(), drand48()) - unitSphere;
    } while (p.squaredLength() >= 1);
    return p;
}

Vec3
randomInUnitDisk()
{
    auto p = Vec3();
    const auto unitDisk = Vec3(1, 1, 0);
    do {
        p = 2 * Vec3(drand48(), drand48(), 0) - unitDisk;
    } while (p.dot(p) >= 1);
    return p;
}

Vec3
reflect(const Vec3& v, const Vec3& normal)
{
    return v - 2 * v.dot(normal) * normal;
}

bool
refract(const Vec3& v, const Vec3& normal, float ni_over_nt, Vec3& refracted)
{
    auto uv = v.normalized();
    auto dt = uv.dot(normal);
    auto discriminant = 1 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant <= 0) {
        return false;
    }

    refracted = ni_over_nt * (uv - normal * dt) - normal*sqrt(discriminant);
    return true;
}

float
toRadians(float degrees)
{
    return degrees * M_PI / 180;
}

float
toDegrees(float radians)
{
    return radians * 180 / M_PI;
}

}
