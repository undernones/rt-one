//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_SPHERE_H
#define GEOM_SPHERE_H

#include <embree2/rtcore_ray.h>

#include <geom/Vec3.h>

namespace render
{

class Sphere
{
public:
    Sphere();
    Sphere(const geom::Vec3& center, float radius);
    ~Sphere() = default;

    bool hit(RTCRay& ray) const;

    geom::Vec3 center() const { return mCenter; }
    float radius() const { return mRadius; }

    std::tuple<float, float> uv(const geom::Vec3& p) const;

private:
    geom::Vec3 mCenter;
    float mRadius;
};

}

#endif // GEOM_SPHERE_H
