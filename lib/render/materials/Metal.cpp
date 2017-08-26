//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Metal.h"

#include <geom/Utils.h>

namespace
{

template <typename T>
inline T
clamp(T const& value, T const& min, T const& max)
{
    return std::max(min, std::min(value, max));
}

}
namespace render
{

Metal::Metal(const geom::Vec3& albedo, float fuzziness)
    : Material()
    , mAlbedo(albedo)
    , mFuzziness(clamp(fuzziness, 0.f, 1.f))
{
}

bool
Metal::scatter(const RTCRay& rayIn, geom::Vec3& attenuation, RTCRay& scattered) const
{
    auto reflected = geom::reflect(rayIn.dir, rayIn.Ng);

    const auto hitPoint = geom::pointAlongRay(rayIn.org, rayIn.dir, rayIn.tfar);
    scattered = geom::newRay(hitPoint, reflected + fuzziness() * geom::randomInUnitSphere(), rayIn.time);
    attenuation = albedo();

    return geom::Vec3(scattered.dir).dot(rayIn.Ng) > 0;
}

}
