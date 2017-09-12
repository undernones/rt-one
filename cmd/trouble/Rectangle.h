//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_RECTANGLE_H
#define TROUBLE_RECTANGLE_H

#include "Ray.h"

// XY: Normal positive in Z
// XZ: Normal positive in Y
// YZ: Normal positive in X
enum class Plane { XY, XZ, YZ };

struct RectYZ
{
public:
    RectYZ(float x, float ymin, float ymax, float zmin, float zmax);

    unsigned commit(RTCDevice device, RTCScene scene);

    float x, ymin, ymax, zmin, zmax;
    unsigned geomID;

private:
    static void intersectFunc(void* userPtr,   /*!< pointer to user data */
                              RTCRay& rtcRay,  /*!< ray to intersect */
                              size_t item      /*!< item to intersect */);

    RTCScene localScene;
};

#endif // TROUBLE_RECTANGLE_H
