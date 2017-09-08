//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_SPHERE_H
#define GEOM_SPHERE_H

#include <render/Hitable.h>

#include <tuple>

#include <geom/Vec3.h>

namespace render
{

class Sphere : public Hitable
{
public:
    Sphere();
    Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>& material);
    Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>&& material);
    Sphere(const geom::Vec3& center0, const geom::Vec3& center1, float t0, float t1, float radius, std::shared_ptr<Material>& material);
    Sphere(const geom::Vec3& center0, const geom::Vec3& center1, float t0, float t1, float radius, std::shared_ptr<Material>&& material);

    virtual ~Sphere() = default;

    virtual bool hit(Ray& ray) const;
    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

    geom::Vec3 center(float t) const;
    float radius() const { return mRadius; }
    float t0() const { return mT0; }
    float t1() const { return mT1; }

    /// Returns the uv coordinates of the given point. Assumes the point is actually on the surface of the sphere at the given time.
    std::tuple<float, float> uv(const geom::Vec3& p, float t) const;

private:
    geom::Vec3 mCenter0;
    geom::Vec3 mCenter1;
    float mT0;
    float mT1;
    float mRadius;
};

}

#endif // GEOM_SPHERE_H
