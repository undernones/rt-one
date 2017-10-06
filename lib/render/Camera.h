//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include <geom/Vec3.h>
#include <render/Ray.h>

namespace render
{

class Camera
{
public:
    Camera() = default;
    Camera(const geom::Vec3& position, const geom::Vec3& lookAt, const geom::Vec3& up, float fov, int width, int height, float aperture, float focusDistance, float t0, float t1);

    Ray getRay(float s, float t) const;
    Ray8 getRays(const std::array<float, 8>& s, const std::array<float, 8>& t) const;

    const geom::Vec3& position() const { return mPosition; }
    const geom::Vec3& lookAt() const { return mLookAt; }
    const geom::Vec3& up() const { return mUpVector; }

    const geom::Vec3& u() const { return mU; }
    const geom::Vec3& v() const { return mV; }
    const geom::Vec3& w() const { return mW; }

    float fov() const { return mFov; }

    int width() const { return mWidth; }
    int height() const { return mHeight; }

    float aperture() const { return mLensRadius * 2; }
    float focusDistance() const { return mFocusDistance; }
    float time0() const { return mTime0; }
    float time1() const { return mTime1; }

private:
    geom::Vec3 mPosition;
    geom::Vec3 mLowerLeft;
    geom::Vec3 mHorizontal;
    geom::Vec3 mVertical;
    geom::Vec3 mU, mV, mW;
    int mWidth;
    int mHeight;
    float mLensRadius;
    float mTime0, mTime1;

    // The following are only kept around for reference
    geom::Vec3 mLookAt;
    geom::Vec3 mUpVector;
    float mFov;
    float mFocusDistance;
};

}

#endif // RENDER_CAMERA_H
