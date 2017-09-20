//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_GRADIENT_TEXTURE_H
#define RENDER_GRADIENT_TEXTURE_H

#include <render/Texture.h>

namespace render
{

class GradientTexture : public Texture
{
public:
    GradientTexture(const geom::Vec3& color1, const geom::Vec3& color2);

    virtual geom::Vec3 value(const geom::Vec2& uv, const geom::Vec3& p) const;

private:
    geom::Vec3 mColor1;
    geom::Vec3 mColor2;
};

}

#endif // RENDER_GRADIENT_TEXTURE_H
