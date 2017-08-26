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

    virtual geom::Vec3 value(float u, float v, const geom::Vec3& p) const;

private:
    float mScale;
};

}

#endif // RENDER_PERLIN_TEXTURE_H
