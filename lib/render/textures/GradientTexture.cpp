//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "GradientTexture.h"

namespace render
{

GradientTexture::GradientTexture(const geom::Vec3& color1, const geom::Vec3& color2)
    : Texture()
    , mColor1(color1)
    , mColor2(color2)
{
}

geom::Vec3
GradientTexture::value(const geom::Vec2& uv, const geom::Vec3& p) const
{
    auto t = uv.v();
    auto result = (1 - t) * mColor1 + t * mColor2;
    return result;
}

}
