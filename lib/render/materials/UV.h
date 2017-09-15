//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_UV_MATERIAL_H
#define RENDER_UV_MATERIAL_H

#include <render/Material.h>

namespace render
{

class UV : public Material
{
public:
    UV();

    virtual bool scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const;
};

}

#endif // RENDER_UV_MATERIAL_H
