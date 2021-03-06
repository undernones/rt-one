//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_DIELECTRIC_H
#define RENDER_DIELECTRIC_H

#include <render/Material.h>

namespace render
{

class Dielectric : public Material
{
public:
    Dielectric(const geom::Vec3& attenuation, float refractIndex);

    virtual bool scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const;

    const geom::Vec3& attenuation() const { return mAttenuation; }
    float refractIndex() const { return mRefractIndex; }

private:
    geom::Vec3 mAttenuation;
    float mRefractIndex;
};

}

#endif // RENDER_DIELECTRIC_H
