//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_PERLIN_TEXTURE_H
#define RENDER_PERLIN_TEXTURE_H

#include "Texture.h"

namespace render
{

class PerlinTexture : public Texture
{
public:
    PerlinTexture();
    PerlinTexture(float scale);

    virtual geom::Vec3 value(const geom::Vec2& uv, const geom::Vec3& p) const;

private:
    float mScale;
};

}

#endif // RENDER_PERLIN_TEXTURE_H
