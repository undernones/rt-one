//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_ENVIRONMENT_MAP_H
#define RENDER_ENVIRONMENT_MAP_H

#include <memory>
#include <geom/Vec3.h>
#include <render/Texture.h>

namespace render
{

class EnvironmentMap
{
public:
    EnvironmentMap(const std::shared_ptr<Texture>& texture);

    geom::Vec3 value(const geom::Vec3& direction) const;

private:
    std::shared_ptr<Texture> mTexture;
};

}

#endif // RENDER_ENVIRONMENT_MAP_H
