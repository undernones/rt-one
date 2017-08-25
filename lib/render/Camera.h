//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include <geom/Ray.h>
#include <geom/Vec3.h>

namespace render
{

class Camera
{
public:
    Camera() = default;
    Camera(const geom::Vec3& position, const geom::Vec3& lookAt, const geom::Vec3& up, float fov, int width, int height, float aperture, float focusDistance);

    RTCRay getRay(float s, float t);

    int width() const { return mWidth; }
    int height() const { return mHeight; }

private:
    geom::Vec3 mOrigin;
    geom::Vec3 mLowerLeft;
    geom::Vec3 mHorizontal;
    geom::Vec3 mVertical;
    geom::Vec3 mU, mV, mW;
    int mWidth;
    int mHeight;
    float mLensRadius;
};

}

#endif // RENDER_CAMERA_H
