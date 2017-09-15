//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_CONSTANT_TEXTURE_H
#define RENDER_CONSTANT_TEXTURE_H

#include "Texture.h"

namespace render
{

class ConstantTexture : public Texture
{
public:
    ConstantTexture();
    ConstantTexture(const geom::Vec3& color);

    virtual geom::Vec3 value(const geom::Vec2& uv, const geom::Vec3& p) const;

private:
    geom::Vec3 mColor;
};

}

#endif // RENDER_CONSTANT_TEXTURE_H
