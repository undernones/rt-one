//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_SPHERE_H
#define TROUBLE_SPHERE_H

#include <tuple>
#include <geom/AABB.h>
#include <geom/Vec3.h>

#include "Hitable.h"
#include "Ray.h"

struct Sphere : public Hitable
{
public:
    Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>& material);
    Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>&& material);

    bool hit(Ray& ray) const;
    bool bbox(float t0, float t1, geom::AABB& bbox) const;
    std::tuple<float, float> uv(const geom::Vec3& p, float t) const;

    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

    geom::Vec3 center;
    float radius;
};

#endif // TROUBLE_SPHERE_H
