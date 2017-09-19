//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "EnvironmentMap.h"

#include <math.h>

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
    auto unitDir = direction.normalized();

    auto phi = std::atan(unitDir.x() / unitDir.z());

    if (unitDir.z() < 0) {
        phi += M_PI;
    } else if (unitDir.x() < 0) {
        phi += M_PI * 2;
    }
    auto u = phi / (2 * M_PI);
    auto v = 0.5 * (unitDir.y() + 1.0);

    auto result = mTexture->value(geom::Vec2(u, v), geom::Vec3());
    return result;
}

}
