//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_TEXTURE_H
#define TROUBLE_TEXTURE_H

#include <geom/Vec3.h>

class Texture
{
public:
    virtual geom::Vec3 value(float u, float v, const geom::Vec3& p) const = 0;
};

#endif // TROUBLE_TEXTURE_H
