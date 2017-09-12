//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_PERLIN_TEXTURE_H
#define TROUBLE_PERLIN_TEXTURE_H

#include "Texture.h"

class PerlinTexture : public Texture
{
public:
    PerlinTexture();
    PerlinTexture(float scale);

    virtual geom::Vec3 value(float u, float v, const geom::Vec3& p) const;

private:
    float mScale;
};

#endif // TROUBLE_PERLIN_TEXTURE_H
