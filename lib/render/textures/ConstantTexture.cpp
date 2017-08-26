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
ConstantTexture::value(float u, float v, const geom::Vec3& p) const
{
    return mColor;
}

}
