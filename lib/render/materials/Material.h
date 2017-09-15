//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_MATERIAL_H
#define RENDER_MATERIAL_H

#include <geom/Vec2.h>
#include <geom/Vec3.h>
#include <render/Ray.h>

namespace render
{

class Material
{
public:
    virtual bool scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const = 0;
    virtual geom::Vec3 emitted(const geom::Vec2& uv, const geom::Vec3& p) const;
};

}

#endif // RENDER_MATERIAL_H
