//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_LAMBERTIAN_H
#define RENDER_LAMBERTIAN_H

#include <render/Material.h>
#include <render/Texture.h>

namespace render
{

class Lambertian : public Material
{
public:
    Lambertian(const std::shared_ptr<Texture>& albedo);

    virtual bool scatter(const Ray& rayIn, geom::Vec3& attenuation, Ray& scattered) const;

    const std::shared_ptr<Texture>& albedo() const { return mAlbedo; }

private:
    std::shared_ptr<Texture> mAlbedo;
};

}
#endif // RENDER_LAMBERTIAN_H
