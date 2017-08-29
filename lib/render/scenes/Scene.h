//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <embree2/rtcore_ray.h>
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
    static void sphereBoundsFunc(void* userPtr,         /*!< pointer to user data */
                                 void* geomUserPtr,     /*!< pointer to geometry user data */
                                 size_t item,           /*!< item to calculate bounds for */
                                 RTCBounds* bounds_o    /*!< returns calculated bounds */);

    static void RTCSphereIntersectFunc(void* userPtr,   /*!< pointer to user data */
                                       RTCRay& ray,     /*!< ray to intersect */
                                       size_t item      /*!< item to intersect */);

    void commit();

    RTCDevice mDevice;
    RTCScene mScene;
    Camera mCamera;
    std::vector<Sphere> mSpheres;
    unsigned mSphereGeomId;
};

}

#endif // RENDER_SCENE_H
