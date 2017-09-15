//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Isotropic.h"

#include <geom/Utils.h>

namespace render
{

Isotropic::Isotropic(const std::shared_ptr<Texture>& albedo)
    : Material()
    , mAlbedo(albedo)
{
}

bool
Isotropic::scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const
{
    auto hitPoint = rayIn.pointAt(rayIn.tfar);
    scattered = Ray(hitPoint, geom::randomInUnitSphere(), rayIn.time);
    attenuation = mAlbedo->value(rayIn.uv, hitPoint);
    return true;
}

}
