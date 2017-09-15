//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Material.h"

namespace render
{

geom::Vec3
Material::emitted(const geom::Vec2& uv, const geom::Vec3& p) const
{
    return geom::Vec3(0, 0, 0);
}

}
