//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_CHECKER_TEXTURE_H
#define RENDER_CHECKER_TEXTURE_H

#include <memory>

#include "Texture.h"

namespace render
{

class CheckerTexture3D : public Texture
{
public:
    CheckerTexture3D(std::shared_ptr<Texture> texture0, std::shared_ptr<Texture> texture1);

    virtual geom::Vec3 value(float u, float v, const geom::Vec3& p) const;

private:
    std::shared_ptr<Texture> mTexture0;
    std::shared_ptr<Texture> mTexture1;
};

}

#endif // RENDER_CHECKER_TEXTURE_H
