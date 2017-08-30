//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "PerlinTexture.h"

#include "Perlin.h"

namespace render
{

PerlinTexture::PerlinTexture()
    : PerlinTexture(1.f)
{
}

PerlinTexture::PerlinTexture(float scale)
    : mScale(scale)
{
}

geom::Vec3
PerlinTexture::value(float u, float v, const geom::Vec3& p) const
{
//    return geom::Vec3(1, 1, 1) * Perlin::noise(mScale * p);
    return geom::Vec3(1, 1, 1) * 0.5 * (1 + sin(mScale * p.z() + 10*Perlin::turbulence(p)));
}

}
