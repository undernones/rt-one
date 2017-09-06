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
Metal::scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const
{
    auto reflected = geom::reflect(rayIn.direction, rayIn.normal);

    const auto hitPoint = rayIn.pointAt(rayIn.tfar);
    scattered = Ray(hitPoint, reflected + fuzziness() * geom::randomInUnitSphere(), rayIn.time);
    attenuation = albedo();

    return geom::Vec3(scattered.direction).dot(rayIn.normal) > 0;
}

}
