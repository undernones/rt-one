//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_DIELECTRIC_H
#define TROUBLE_DIELECTRIC_H

#include "Material.h"

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

#endif // TROUBLE_DIELECTRIC_H
