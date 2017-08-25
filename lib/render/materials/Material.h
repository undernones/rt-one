//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_MATERIAL_H
#define RENDER_MATERIAL_H

#include <geom/Vec3.h>
#include <geom/Ray.h>

namespace render
{

class Material
{
public:
    virtual bool scatter(const RTCRay& rayIn, geom::Vec3& attenuation, RTCRay& scattered) const = 0;
    virtual geom::Vec3 emitted(float u, float v, const geom::Vec3& p) const;
};

}

#endif // RENDER_MATERIAL_H