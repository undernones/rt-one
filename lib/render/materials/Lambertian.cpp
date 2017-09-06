//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Lambertian.h"

#include <geom/Utils.h>
#include <render/Ray.h>

namespace render
{

Lambertian::Lambertian(const std::shared_ptr<Texture>& albedo)
    : Material()
    , mAlbedo(albedo)
{
}

bool
Lambertian::scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const
{
    const auto hitPoint = rayIn.pointAt(rayIn.tfar);
    const auto target = hitPoint + rayIn.normal + geom::randomInUnitSphere();
    scattered = Ray(hitPoint, target - hitPoint, rayIn.time);
    attenuation = mAlbedo->value(rayIn.u, rayIn.v, hitPoint);
    return true;
}

}
