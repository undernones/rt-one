//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "AABB.h"

#include <float.h>

namespace
{

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

}

namespace geom
{

AABB::AABB()
    : AABB(Vec3(FLT_MAX), Vec3(-FLT_MAX))
{
}

AABB::AABB(const Vec3& min, const Vec3& max)
    : mMin(min)
    , mMax(max)
{
}

AABB::AABB(const AABB& a, const AABB& b)
{
    mMin = Vec3(ffmin(a.min().x(), b.min().x()),
                ffmin(a.min().y(), b.min().y()),
                ffmin(a.min().z(), b.min().z()));

    mMax = Vec3(ffmax(a.max().x(), b.max().x()),
                ffmax(a.max().y(), b.max().y()),
                ffmax(a.max().z(), b.max().z()));
}

void
AABB::includePoint(const Vec3& pt)
{
    mMin = Vec3(ffmin(mMin.x(), pt.x()),
                ffmin(mMin.y(), pt.y()),
                ffmin(mMin.z(), pt.z()));

    mMax = Vec3(ffmax(mMax.x(), pt.x()),
                ffmax(mMax.y(), pt.y()),
                ffmax(mMax.z(), pt.z()));
}

}
