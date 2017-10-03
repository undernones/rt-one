//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_UTILS_H
#define GEOM_UTILS_H

#include "Vec2.h"
#include "Vec3.h"

namespace geom
{

Vec3 randomInUnitSphere();
Vec3 randomInUnitDisk();

Vec3 reflect(const Vec3& v, const Vec3& normal);
bool refract(const Vec3& v, const Vec3& normal, float ni_over_nt, Vec3& refracted);

float toRadians(float degrees);
float toDegrees(float radians);

template <typename T>
T lerp(const T& a, const T& b, float t)
{
    return (1 - t) * a + t * b;
}

Vec2 cartesianToPolar(const Vec3& cartesian);
Vec3 polarToCartesian(const Vec2& polar);

}

#endif // GEOM_UTILS_H
