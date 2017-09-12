//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_SPHERE_H
#define TROUBLE_SPHERE_H

#include <tuple>
#include <geom/Vec3.h>

#include "Renderable.h"

struct Sphere : public Renderable
{
public:
    Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>& material);
    Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>&& material);

    bool intersect(Ray& ray) const;
    std::tuple<float, float> uv(const geom::Vec3& p, float t) const;

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

private:
    geom::Vec3 mCenter;
    float mRadius;
};

#endif // TROUBLE_SPHERE_H
