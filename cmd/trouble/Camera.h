//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_CAMERA_H
#define TROUBLE_CAMERA_H

#include <embree2/rtcore_ray.h>
#include <geom/Vec3.h>

#include "Ray.h"

class Camera
{
public:
    Camera() = default;
    Camera(const geom::Vec3& position, const geom::Vec3& lookAt, const geom::Vec3& up, float fov, int width, int height, float aperture, float focusDistance, float t0, float t1);

    Ray getRay(float s, float t) const;

private:
    geom::Vec3 mOrigin;
    geom::Vec3 mLowerLeft;
    geom::Vec3 mHorizontal;
    geom::Vec3 mVertical;
    geom::Vec3 mU, mV, mW;
    int mWidth;
    int mHeight;
    float mLensRadius;
    float mTime0, mTime1;
};

#endif // TROUBLE_CAMERA_H
