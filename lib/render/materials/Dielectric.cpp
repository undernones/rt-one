//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Dielectric.h"

#include <geom/Utils.h>

namespace
{

float
schlick(float cosine, float refractIndex)
{
    auto r0 = (1 - refractIndex) / (1 + refractIndex);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine), 5);
}

}

namespace render
{

Dielectric::Dielectric(const geom::Vec3& attenuation, float refractIndex)
    : Material()
    , mAttenuation(attenuation)
    , mRefractIndex(refractIndex)
{
}

bool
Dielectric::scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const
{
    const auto inDirection = rayIn.direction.normalized();
    const auto normal = rayIn.normal.normalized();

    auto outwardNormal = geom::Vec3();
    auto reflected = geom::reflect(inDirection, normal);
    auto ni_over_nt = 0.f;
    attenuation = mAttenuation;
    auto reflectProb = 1.f;
    auto cosine = 0.f;

    if (inDirection.dot(normal) > 0) {
        outwardNormal = -normal;
        ni_over_nt = mRefractIndex;
        cosine = mRefractIndex * inDirection.dot(normal);
    } else {
        outwardNormal = normal;
        ni_over_nt = 1.f / mRefractIndex;
        cosine = -inDirection.dot(normal);
    }

    auto refracted = geom::Vec3();
    if (geom::refract(inDirection, outwardNormal, ni_over_nt, refracted)) {
        reflectProb = schlick(cosine, mRefractIndex);
    }

    auto hitPoint = rayIn.pointAt(rayIn.tfar);
    if (geom::rand() < reflectProb) {
        scattered = Ray(hitPoint, reflected, rayIn.time);
    } else {
        scattered = Ray(hitPoint, refracted, rayIn.time);
    }
    return true;
}

}
