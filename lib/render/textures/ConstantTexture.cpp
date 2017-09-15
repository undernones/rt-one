//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "ConstantTexture.h"

namespace render
{

ConstantTexture::ConstantTexture()
    : ConstantTexture(geom::Vec3())
{
}

ConstantTexture::ConstantTexture(const geom::Vec3& color)
    : Texture()
    , mColor(color)
{
}

geom::Vec3
ConstantTexture::value(const geom::Vec2& uv, const geom::Vec3& p) const
{
    return mColor;
}

}
