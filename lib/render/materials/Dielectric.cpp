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
Dielectric::scatter(const RTCRay& rayIn, geom::Vec3& attenuation, RTCRay& scattered) const
{
    auto inDirection = geom::Vec3(rayIn.dir);
    auto normal = geom::Vec3(rayIn.Ng);

    auto outwardNormal = geom::Vec3();
    auto reflected = geom::reflect(inDirection, normal);
    auto ni_over_nt = 0.f;
    attenuation = mAttenuation;
    auto reflectProb = 1.f;
    auto cosine = 0.f;

    if (inDirection.dot(normal) > 0) {
        outwardNormal = -normal;
        ni_over_nt = mRefractIndex;
        cosine = mRefractIndex * inDirection.dot(normal) / inDirection.length();
    } else {
        outwardNormal = normal;
        ni_over_nt = 1.f / mRefractIndex;
        cosine = -inDirection.dot(normal) / inDirection.length();
    }

    auto refracted = geom::Vec3();
    if (geom::refract(inDirection, outwardNormal, ni_over_nt, refracted)) {
        reflectProb = schlick(cosine, mRefractIndex);
    }

    auto hitPoint = geom::pointAlongRay(rayIn.org, rayIn.dir, rayIn.tfar);
    if (drand48() < reflectProb) {
        scattered = geom::newRay(hitPoint, reflected, rayIn.time);
    } else {
        scattered = geom::newRay(hitPoint, refracted, rayIn.time);
    }
    return true;
}

}
