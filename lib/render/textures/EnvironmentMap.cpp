//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "EnvironmentMap.h"

#include <geom/Utils.h>

namespace render
{

EnvironmentMap::EnvironmentMap(const std::shared_ptr<Texture>& texture)
    : mTexture(texture)
{
}

geom::Vec3
EnvironmentMap::value(const geom::Vec3& direction) const
{
    auto spherical = geom::cartesianToSpherical(direction);
    auto u = (spherical.v() + M_PI) / (2 * M_PI);
    auto v = 1.0 - (spherical.w() * M_1_PI);
    auto result = mTexture->value(geom::Vec2(u, v), geom::Vec3());
    return result;
}

}
