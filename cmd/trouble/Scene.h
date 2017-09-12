//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_SCENE_H
#define TROUBLE_SCENE_H

#include <embree2/rtcore_ray.h>
#include <vector>

#include "Camera.h"

// TODO: get rid of these
#include "Rectangle.h"
#include "Sphere.h"

class Scene
{
public:
    Scene();
    virtual ~Scene();

    const RTCScene& rtcScene() const { return mScene; }
    const Camera& camera() const { return mCamera; }
    void setCamera(const Camera& c) { mCamera = c; }

protected:
    void commit();

    RTCDevice mDevice;
    RTCScene mScene;
    Camera mCamera;
    std::vector<unsigned> mGeomIds;
};

#endif // TROUBLE_SCENE_H
