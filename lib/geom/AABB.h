//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_AABB_H
#define GEOM_AABB_H

#include "Ray.h"
#include "Vec3.h"

namespace geom
{

class AABB
{
public:
    AABB();
    AABB(const Vec3& min, const Vec3& max);
    AABB(const AABB& a, const AABB& b);

    void includePoint(const Vec3& pt);

    const Vec3& min() const { return mMin; }
    const Vec3& max() const { return mMax; }

    bool hit(const RTCRay& ray, float tmin, float tmax) const;

private:
    Vec3 mMin;
    Vec3 mMax;
};

}

#endif // GEOM_AABB_H
