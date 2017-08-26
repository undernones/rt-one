//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Sphere.h"

#include <geom/Ray.h>

namespace render
{

Sphere::Sphere()
    : Sphere(geom::Vec3(0, 0, 0), 1.f, std::shared_ptr<Material>(nullptr))
{
}

Sphere::Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>& material)
    : Sphere(center, radius, std::move(material))
{
}

Sphere::Sphere(const geom::Vec3& center, float radius, std::shared_ptr<Material>&& material)
    : Sphere(center, center, 0.f, 1.f, radius, material)
{
}

Sphere::Sphere(const geom::Vec3& center0, const geom::Vec3& center1, float t0, float t1, float radius, std::shared_ptr<Material>& material)
    : Sphere(center0, center1, t0, t1, radius, std::move(material))
{
}

Sphere::Sphere(const geom::Vec3& center0, const geom::Vec3& center1, float t0, float t1, float radius, std::shared_ptr<Material>&& material)
    : mCenter0(center0)
    , mCenter1(center1)
    , mT0(t0)
    , mT1(t1)
    , mRadius(radius)
    , mMaterial(material)
{
}

bool
Sphere::hit(RTCRay& ray) const
{
    auto origin = geom::Vec3(ray.org);
    auto direction = geom::Vec3(ray.dir);

    auto center = this->center(ray.time);

    auto oc = origin - center;
    auto a = direction.dot(direction);
    auto b = oc.dot(direction);
    auto c = oc.dot(oc) - mRadius * mRadius;
    auto discriminant = b*b - a*c;

    auto isHit = false;
    auto t = 0.f;

    if (discriminant > 0) {
        t = (-b - sqrt(discriminant)) / a;
        if (ray.tnear < t && t < ray.tfar) {
            isHit = true;
        } else {
            t = (-b + sqrt(discriminant)) / a;
            if (ray.tnear < t && t < ray.tfar) {
                isHit = true;
            }
        }
    }

    if (isHit) {
        auto hitPoint = geom::pointAlongRay(ray.org, ray.dir, t);
        auto normal = (hitPoint - center) / mRadius;

        ray.tfar = t;
        ray.Ng[0] = normal.x();
        ray.Ng[1] = normal.y();
        ray.Ng[2] = normal.z();
    }

    return isHit;
}

bool
Sphere::bbox(float t0, float t1, geom::AABB& bbox) const
{
    auto offset = geom::Vec3(mRadius);
    auto aabb0 = geom::AABB(mCenter0 - offset, mCenter0 + offset);
    auto aabb1 = geom::AABB(mCenter1 - offset, mCenter1 + offset);
    bbox = geom::AABB(aabb0, aabb1);
    return true;
}

geom::Vec3
Sphere::center(float t) const
{
    auto blend = (t - mT0) / (mT1 - mT0);
    return mCenter0 + blend * (mCenter1 - mCenter0);
}

std::tuple<float, float>
Sphere::uv(const geom::Vec3 &p, float t) const
{
    auto normalizedPt = (p - center(t)) / mRadius;

    auto phi = atan2f(normalizedPt.z(), normalizedPt.x());
    auto theta = asinf(normalizedPt.y());

    auto u = 1.f - (phi + M_PI) / (2 * M_PI);
    auto v = (theta + M_PI_2) / M_PI;
    return { u, v };
}

}
