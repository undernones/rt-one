//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_SPHERE_H
#define GEOM_SPHERE_H

#include <embree2/rtcore_ray.h>
#include <memory>

#include <geom/Vec3.h>

#include "Material.h"

namespace render
{

class Sphere
{
public:
    Sphere();
    Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>& material);
    Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>&& material);
    ~Sphere() = default;

    bool hit(RTCRay& ray) const;

    geom::Vec3 center() const { return mCenter; }
    float radius() const { return mRadius; }

    std::tuple<float, float> uv(const geom::Vec3& p) const;

    const std::shared_ptr<Material> material() const { return mMaterial; }

private:
    geom::Vec3 mCenter;
    float mRadius;

    std::shared_ptr<Material> mMaterial;
};

}

#endif // GEOM_SPHERE_H
