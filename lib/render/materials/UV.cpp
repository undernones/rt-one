//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "UV.h"

#include <geom/Utils.h>

namespace render
{

UV::UV()
    : Material()
{
}

bool
UV::scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const
{
    const auto hitPoint = rayIn.pointAt(rayIn.tfar);
    const auto target = hitPoint + rayIn.normal.normalized() + geom::randomInUnitSphere();
    scattered = Ray(hitPoint, target - hitPoint, rayIn.time);
    attenuation = geom::Vec3(rayIn.uv.u(), rayIn.uv.v(), 0);
    return true;
}

}
