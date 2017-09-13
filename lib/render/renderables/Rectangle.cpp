//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Rectangle.h"

#include <geom/AABB.h>

namespace
{

const auto EPSILON = 1e-4;

struct Vertex   { float x, y, z, a; };
struct Triangle { int v0, v1, v2; };

void
boundsFunc(void* userPtr,         /*!< pointer to user data */
           void* geomUserPtr,     /*!< pointer to geometry user data */
           size_t item,           /*!< item to calculate bounds for */
           RTCBounds* bounds_o    /*!< returns calculated bounds */)
{
    // Assume we can dereference userPtr
    const auto rect = static_cast<render::Rectangle*>(userPtr);

    auto bbox = geom::AABB();
    if (!rect->bbox(0, 1, bbox)) {
        return;
    }

    auto lower = bbox.min();
    auto upper = bbox.max();

    bounds_o->lower_x = lower.x();
    bounds_o->lower_y = lower.y();
    bounds_o->lower_z = lower.z();

    bounds_o->upper_x = upper.x();
    bounds_o->upper_y = upper.y();
    bounds_o->upper_z = upper.z();
}

}

namespace render
{

Rectangle::Rectangle(Plane plane, float left, float right, float bottom, float top, float offset, std::shared_ptr<Material>& material)
    : Rectangle(plane, left, right, bottom, top, offset, std::move(material))
{
}

Rectangle::Rectangle(Plane plane, float left, float right, float bottom, float top, float offset, std::shared_ptr<Material>&& material)
    : Renderable(material)
    , mLocalScene(nullptr)
    , mPlane(plane)
    , mLeft(left)
    , mRight(right)
    , mBottom(bottom)
    , mTop(top)
    , mOffset(offset)
{
}

Rectangle::~Rectangle()
{
    if (mLocalScene != nullptr) {
        rtcDeleteScene(mLocalScene);
    }
}

std::vector<unsigned>
Rectangle::commit(RTCDevice device, RTCScene scene)
{
    // First register a new scene consisting of just the rectangle's triangle mesh.
    mLocalScene = rtcDeviceNewScene(device, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
    auto meshGeomId = rtcNewTriangleMesh(mLocalScene, RTC_GEOMETRY_STATIC, 2, 4);

    // XY: Normal positive in Z
    // XZ: Normal positive in Y
    // YZ: Normal positive in X
    auto verts = (Vertex*)rtcMapBuffer(mLocalScene, meshGeomId, RTC_VERTEX_BUFFER);
    switch (mPlane) {
        case Plane::XY:
            verts[0] = { mLeft,  mBottom, mOffset, 1 };
            verts[1] = { mRight, mBottom, mOffset, 1 };
            verts[2] = { mRight, mTop,    mOffset, 1 };
            verts[3] = { mLeft,  mTop,    mOffset, 1 };
            break;

        case Plane::XZ:
            verts[0] = { mLeft,  mOffset, mTop,    1 };
            verts[1] = { mRight, mOffset, mTop,    1 };
            verts[2] = { mRight, mOffset, mBottom, 1 };
            verts[3] = { mLeft,  mOffset, mBottom, 1 };
            break;

        case Plane::YZ:
            verts[0] = { mOffset, mLeft,  mBottom, 1 };
            verts[1] = { mOffset, mRight, mBottom, 1 };
            verts[2] = { mOffset, mRight, mTop,    1 };
            verts[3] = { mOffset, mLeft,  mTop,    1 };
            break;
    }
    rtcUnmapBuffer(mLocalScene, meshGeomId, RTC_VERTEX_BUFFER);

    auto tris = (Triangle*)rtcMapBuffer(mLocalScene, meshGeomId, RTC_INDEX_BUFFER);
    tris[0] = { 0, 1, 3 };
    tris[1] = { 1, 2, 3 };
    rtcUnmapBuffer(mLocalScene, meshGeomId, RTC_INDEX_BUFFER);

    rtcCommit(mLocalScene);

    // Then register a user geometry so we can intersect against this mesh while giving ourselves
    // an opportunity to set its material.
    mGeomId = rtcNewUserGeometry3(scene, RTC_GEOMETRY_STATIC, 1);
    rtcSetUserData(scene, mGeomId, this);
    rtcSetBoundsFunction2(scene, mGeomId, boundsFunc, this);
    rtcSetIntersectFunction(scene, mGeomId, intersectFunc);
    return std::vector<unsigned>( { mGeomId });
}

void
Rectangle::intersectFunc(void* userPtr,   /*!< pointer to user data */
                         RTCRay& rtcRay,  /*!< ray to intersect */
                         size_t item      /*!< item to intersect */)
{
    // Assume we can dereference userPtr
    const auto rect = static_cast<render::Rectangle*>(userPtr);

    const auto geomID = rtcRay.geomID;
    rtcRay.geomID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect(rect->mLocalScene, rtcRay);

    if (rtcRay.geomID != RTC_INVALID_GEOMETRY_ID) {
        rtcRay.geomID = rect->mGeomId;
        auto& ray = (render::Ray&)rtcRay;
        ray.material = rect->material().get();
        ray.normal = -ray.normal.normalized();
    } else {
        rtcRay.geomID = geomID;
    }
}

bool
Rectangle::hit(Ray& ray) const
{
    // Unused
    return false;
}

bool
Rectangle::bbox(float t0, float t1, geom::AABB& bbox) const
{
    geom::Vec3 min, max;
    switch (mPlane) {
        case Plane::XY:
        min = geom::Vec3(mLeft, mBottom, mOffset - EPSILON);
        max = geom::Vec3(mRight, mTop, mOffset + EPSILON);
        break;

        case Plane::XZ:
        min = geom::Vec3(mLeft, mOffset - EPSILON, mBottom);
        max = geom::Vec3(mRight, mOffset + EPSILON, mTop);
        break;

        case Plane::YZ:
        min = geom::Vec3(mOffset - EPSILON, mLeft, mBottom);
        max = geom::Vec3(mOffset + EPSILON, mRight, mTop);
        break;
    }
    bbox = geom::AABB(min, max);
    return true;
}

}
