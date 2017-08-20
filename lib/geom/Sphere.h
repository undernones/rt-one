//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_SPHERE_H
#define GEOM_SPHERE_H

#include "Hitable.h"
#include "Vec3.h"

namespace geom
{

class Sphere : public Hitable
{
public:
    Sphere();
    Sphere(const Vec3& center, float radius);
    ~Sphere() = default;

    virtual bool hit(RTCRay& ray) const;

    Vec3 center() const { return mCenter; }
    float radius() const { return mRadius; }

    std::tuple<float, float> uv(const Vec3& p) const;

private:
    Vec3 mCenter;
    float mRadius;
};

}

#endif // GEOM_SPHERE_H
