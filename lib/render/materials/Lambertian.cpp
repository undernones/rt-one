//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Lambertian.h"

#include <geom/Ray.h>
#include <geom/Utils.h>

namespace render
{

Lambertian::Lambertian(const geom::Vec3& albedo)
    : Material()
    , mAlbedo(albedo)
{
}

bool
Lambertian::scatter(const RTCRay& rayIn, geom::Vec3& attenuation, RTCRay& scattered) const
{
    const auto normal = geom::Vec3(rayIn.Ng);
    const auto hitPoint = geom::pointAlongRay(rayIn.org, rayIn.dir, rayIn.tfar);
    const auto target = hitPoint + normal + geom::randomInUnitSphere();
    scattered = geom::newRay(hitPoint, target - hitPoint, rayIn.time);
    attenuation = albedo();
    return true;
}

}