//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Camera.h"

#include <geom/Utils.h>

namespace render
{

Camera::Camera(const geom::Vec3& origin, const geom::Vec3& lowerLeft, const geom::Vec3& horizontal, const geom::Vec3& vertical)
    : mOrigin(origin)
    , mLowerLeft(lowerLeft)
    , mHorizontal(horizontal)
    , mVertical(vertical)
{
}

//Camera::Camera(const geom::Vec3& position, const geom::Vec3& lookAt, const geom::Vec3& up, float fov, int width, int height)
//    : mOrigin(position)
//    , mLowerLeft(0, 0, 0)
//    , mHorizontal(0, 0, 0)
//    , mVertical(0, 0, 0)
//    , mWidth(width)
//    , mHeight(height)
//{
//    auto aspectRatio = static_cast<float>(width) / height;
//    auto theta = geom::toRadians(fov);
//    auto halfHeight = tan(theta * 0.5);
//    auto halfWidth = aspectRatio * halfHeight;
//
//    mW = (position - lookAt).normalized();
//    mU = up.cross(mW).normalized();
//    mV = mW.cross(mU);
//
//    mLowerLeft = position - halfWidth*mU - halfHeight*mV;
//    mHorizontal = 2 * halfWidth*mU;
//    mVertical = 2 * halfHeight*mV;
//}

RTCRay
Camera::getRay(float s, float t)
{
    auto direction = mLowerLeft + s*mHorizontal + t*mVertical;
    auto result = geom::newRay(mOrigin, direction);
    return result;
}

}
