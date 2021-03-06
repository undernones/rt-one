//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <embree2/rtcore_ray.h>
#include <vector>

#include <render/Camera.h>
#include <render/EnvironmentMap.h>
#include <render/Renderable.h>

namespace render
{

class Material;
class Scene
{
public:
    Scene();
    virtual ~Scene();

    const RTCScene& rtcScene() const { return mScene; }
    const Camera& camera() const { return mCamera; }
    void setCamera(const Camera& c) { mCamera = c; }
    const std::shared_ptr<EnvironmentMap>& environmentMap() const { return mEnvMap; }

protected:
    void commit();

    RTCDevice mDevice;
    RTCScene mScene;
    Camera mCamera;
    std::shared_ptr<EnvironmentMap> mEnvMap;
    std::shared_ptr<Renderable> mRoot;
    std::vector<unsigned> mGeomIds;
};

}

#endif // RENDER_SCENE_H
