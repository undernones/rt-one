//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Camera.h"

#include <geom/Utils.h>

namespace render
{

Camera::Camera(const geom::Vec3& position, const geom::Vec3& lookAt, const geom::Vec3& up, float fov, int width, int height, float aperture, float focusDistance, float t0, float t1)
    : mPosition(position)
    , mLookAt(lookAt)
    , mLowerLeft(0, 0, 0)
    , mHorizontal(0, 0, 0)
    , mVertical(0, 0, 0)
    , mUpVector(up)
    , mFov(fov)
    , mWidth(width)
    , mHeight(height)
    , mLensRadius(aperture * 0.5)
    , mFocusDistance(focusDistance)
    , mTime0(t0)
    , mTime1(t1)
{
    auto aspectRatio = static_cast<float>(width) / height;
    auto theta = geom::toRadians(fov);
    auto halfHeight = tan(theta * 0.5);
    auto halfWidth = aspectRatio * halfHeight;

    mW = (position - lookAt).normalized();
    mU = up.cross(mW).normalized();
    mV = mW.cross(mU);

    mLowerLeft = position - halfWidth*focusDistance*mU - halfHeight*focusDistance*mV - focusDistance*mW;
    mHorizontal = 2 * halfWidth*focusDistance*mU;
    mVertical = 2 * halfHeight*focusDistance*mV;
}

Ray
Camera::getRay(float s, float t) const
{
    auto time = geom::lerp(mTime0, mTime1, drand48());
    auto rd = mLensRadius * geom::randomInUnitDisk();
    auto offset = mU * rd.x() + mV * rd.y();
    auto origin = mPosition + offset;
    auto result = Ray(origin, mLowerLeft + s*mHorizontal + t*mVertical - origin, time);
    return result;
}

}
