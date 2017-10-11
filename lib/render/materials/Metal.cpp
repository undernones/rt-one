//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Metal.h"

#include <algorithm>
#include <geom/Utils.h>

namespace render
{

Metal::Metal(const geom::Vec3& albedo, float fuzziness)
    : Material()
    , mAlbedo(albedo)
    , mFuzziness(std::clamp(fuzziness, 0.f, 1.f))
{
}

bool
Metal::scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const
{
    auto reflected = geom::reflect(rayIn.direction, rayIn.normal.normalized()).normalized();

    const auto hitPoint = rayIn.pointAt(rayIn.tfar);
    scattered = Ray(hitPoint, reflected + fuzziness() * geom::randomInUnitSphere(), rayIn.time);
    attenuation = albedo();

    return geom::Vec3(scattered.direction).dot(rayIn.normal) > 0;
}

}
