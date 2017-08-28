#ifndef RENDER_LIGHT_H
#define RENDER_LIGHT_H

#include <memory>

#include "Material.h"
#include "Texture.h"

namespace render
{

class DiffuseLight : public Material
{
public:
    DiffuseLight(const std::shared_ptr<Texture>& emissions);

    virtual bool scatter(const RTCRay& rayIn, geom::Vec3& attenuation, RTCRay& scattered) const;
    virtual geom::Vec3 emitted(float u, float v, const geom::Vec3& p) const;

private:
    std::shared_ptr<Texture> mEmissions;
};

}

#endif // RENDER_LIGHT_H
