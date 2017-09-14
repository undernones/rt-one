//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_ISOTROPIC_H
#define RENDER_ISOTROPIC_H

#include <memory>

#include <render/Material.h>
#include <render/Texture.h>

namespace render
{

class Isotropic : public Material
{
public:
    Isotropic(const std::shared_ptr<Texture>& albedo);

    virtual bool scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const;

    const std::shared_ptr<Texture>& albedo() const { return mAlbedo; }

private:
    std::shared_ptr<Texture> mAlbedo;
};

}

#endif // RENDER_ISOTROPIC_H
