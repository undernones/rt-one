//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_METAL_H
#define TROUBLE_METAL_H

#include "Material.h"

class Metal : public Material
{
public:
    Metal(const geom::Vec3& albedo, float fuzziness);

    virtual bool scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const;

    const geom::Vec3& albedo() const { return mAlbedo; }
    float fuzziness() const { return mFuzziness; }

private:
    geom::Vec3 mAlbedo;
    float mFuzziness;
};

#endif // TROUBLE_METAL_H
