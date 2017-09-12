//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_DIFFUSE_LIGHT_H
#define TROUBLE_DIFFUSE_LIGHT_H

#include <memory>

#include "Material.h"
#include "Texture.h"

class DiffuseLight : public Material
{
public:
    DiffuseLight(const std::shared_ptr<Texture>& emissions);

    virtual bool scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const;
    virtual geom::Vec3 emitted(float u, float v, const geom::Vec3& p) const;

private:
    std::shared_ptr<Texture> mEmissions;
};

#endif // TROUBLE_DIFFUSE_LIGHT_H
