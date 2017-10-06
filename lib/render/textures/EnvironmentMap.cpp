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

std::array<geom::Vec3, 8>
EnvironmentMap::value8(float dirx[8], float diry[8], float dirz[8]) const
{
    auto result = std::array<geom::Vec3, 8>();
    for (auto i = 0; i < 8; ++i) {
        auto direction = geom::Vec3(dirx[i], diry[i], dirz[i]);
        result[i] = value(direction);
    }
    return result;
}

}
