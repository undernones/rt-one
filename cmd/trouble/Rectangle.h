//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_RECTANGLE_H
#define TROUBLE_RECTANGLE_H

#include <geom/AABB.h>

#include "Renderable.h"

// XY: Normal positive in Z
// XZ: Normal positive in Y
// YZ: Normal positive in X
enum class Plane { XY, XZ, YZ };

struct RectYZ : public Renderable
{
public:
    RectYZ(float x, float ymin, float ymax, float zmin, float zmax, std::shared_ptr<Material>& material);
    RectYZ(float x, float ymin, float ymax, float zmin, float zmax, std::shared_ptr<Material>&& material);

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

private:
    static void intersectFunc(void* userPtr,   /*!< pointer to user data */
                              RTCRay& rtcRay,  /*!< ray to intersect */
                              size_t item      /*!< item to intersect */);

    float x, ymin, ymax, zmin, zmax;
    RTCScene mLocalScene;
};

#endif // TROUBLE_RECTANGLE_H
