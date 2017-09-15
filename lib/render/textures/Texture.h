//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include <memory>
#include <geom/Vec2.h>
#include <geom/Vec3.h>

namespace render
{

class Texture
{
public:
    virtual geom::Vec3 value(const geom::Vec2& uv, const geom::Vec3& p) const = 0;
};

}

#endif // RENDER_TEXTURE_H
