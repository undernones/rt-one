//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_LAMBERTIAN_H
#define RENDER_LAMBERTIAN_H

#include "Material.h"

namespace render
{

class Lambertian : public Material
{
public:
    Lambertian(const geom::Vec3& albedo);

    virtual bool scatter(const RTCRay& rayIn, geom::Vec3& attenuation, RTCRay& scattered) const;

    const geom::Vec3& albedo() const { return mAlbedo; }

private:
    geom::Vec3 mAlbedo;
};

}
#endif // RENDER_LAMBERTIAN_H
