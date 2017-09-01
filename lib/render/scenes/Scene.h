//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <embree2/rtcore_ray.h>
#include <unordered_map>
#include <vector>

#include "Camera.h"
#include "Sphere.h"

namespace render
{

class Material;
class Scene
{
public:
    Scene();
    virtual ~Scene();

    const RTCScene& root() const { return mScene; }
    const Camera& camera() const { return mCamera; }
    void setCamera(const Camera& c) { mCamera = c; }

    std::shared_ptr<Material> material(unsigned geomId, unsigned primId) const;

protected:
    void commit();

    RTCDevice mDevice;
    RTCScene mScene;
    Camera mCamera;
    std::vector<Sphere> mSpheres;
    std::unordered_map<unsigned, std::shared_ptr<Material>> mMaterials;
};

}

#endif // RENDER_SCENE_H
