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
    : mCenter(center)
    , mRadius(radius)
    , mMaterial(material)
{
}

bool
Sphere::hit(RTCRay& ray) const
{
    auto origin = geom::Vec3(ray.org);
    auto direction = geom::Vec3(ray.dir);

    auto oc = origin - mCenter;
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
        auto normal = (hitPoint - mCenter) / mRadius;

        ray.tfar = t;
        ray.Ng[0] = normal.x();
        ray.Ng[1] = normal.y();
        ray.Ng[2] = normal.z();
    }

    return isHit;
}

std::tuple<float, float>
Sphere::uv(const geom::Vec3 &p) const
{
    auto normalizedPt = (p - mCenter) / mRadius;

    auto phi = atan2f(normalizedPt.z(), normalizedPt.x());
    auto theta = asinf(normalizedPt.y());

    auto u = 1.f - (phi + M_PI) / (2 * M_PI);
    auto v = (theta + M_PI_2) / M_PI;
    return { u, v };
}

}
