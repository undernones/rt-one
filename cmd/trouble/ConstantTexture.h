//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_CONSTANT_TEXTURE_H
#define TROUBLE_CONSTANT_TEXTURE_H

#include "Texture.h"

class ConstantTexture : public Texture
{
public:
    ConstantTexture();
    ConstantTexture(const geom::Vec3& color);

    virtual geom::Vec3 value(float u, float v, const geom::Vec3& p) const;

private:
    geom::Vec3 mColor;
};

#endif // TROUBLE_CONSTANT_TEXTURE_H
