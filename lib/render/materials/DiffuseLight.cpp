//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "DiffuseLight.h"

namespace render
{

DiffuseLight::DiffuseLight(const std::shared_ptr<Texture>& emissions)
    : Material()
    , mEmissions(emissions)
{
}

bool
DiffuseLight::scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const
{
    return false;
}

geom::Vec3
DiffuseLight::emitted(const geom::Vec2& uv, const geom::Vec3& p) const
{
    return mEmissions->value(uv, p);
}

}
