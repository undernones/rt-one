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
    // This environment map is spherical, so the strategy is to map the Cartesian coordinates of
    // the direction to polar coordinates and normalize.
    auto polar = geom::cartesianToPolar(direction);
    auto u = (polar.u() + M_PI) / (2 * M_PI);
    auto v = 1.0 - (polar.v() * M_1_PI);
    auto result = mTexture->value(geom::Vec2(u, v), geom::Vec3());
    return result;
}

}
