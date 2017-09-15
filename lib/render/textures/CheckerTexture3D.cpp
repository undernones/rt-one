//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "CheckerTexture3D.h"

namespace render
{

CheckerTexture3D::CheckerTexture3D(std::shared_ptr<Texture> texture0, std::shared_ptr<Texture> texture1)
    : Texture()
    , mTexture0(texture0)
    , mTexture1(texture1)
{
}

geom::Vec3
CheckerTexture3D::value(const geom::Vec2& uv, const geom::Vec3& p) const
{
    auto scaledP = 10 * p;
    auto sineProduct = sin(scaledP.x()) * sin(scaledP.y()) * sin(scaledP.z());
    if (sineProduct < 0) {
        return mTexture0->value(uv, p);
    } else {
        return mTexture1->value(uv, p);
    }
}

}
