//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_MATERIAL_H
#define TROUBLE_MATERIAL_H

#include <geom/Vec3.h>
#include "Ray.h"

class Material
{
public:
    virtual bool scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const = 0;
    virtual geom::Vec3 emitted(float u, float v, const geom::Vec3& p) const;
};

#endif // TROUBLE_MATERIAL_H
