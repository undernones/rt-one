//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_LAMBERTIAN_H
#define TROUBLE_LAMBERTIAN_H

#include "Material.h"
#include "Texture.h"

class Lambertian : public Material
{
public:
    Lambertian(const std::shared_ptr<Texture>& albedo);

    virtual bool scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const;

    const std::shared_ptr<Texture>& albedo() const { return mAlbedo; }

private:
    std::shared_ptr<Texture> mAlbedo;
};

#endif /* Lambertian____FILEEXTENSION___ */
