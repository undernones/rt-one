//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Rectangle.h"

#include <array>
#include <geom/AABB.h>
#include <geom/Vec2.h>

namespace
{

struct Vertex   { float x, y, z, a; };
struct Triangle { int v0, v1, v2; };

const auto TRIANGLES = std::array<Triangle, 2>({
    Triangle({ 0, 1, 3 }),
    Triangle({ 1, 2, 3 }),
});

const auto ST_VALUES = std::array<geom::Vec2, 4>({
    geom::Vec2(0, 0),
    geom::Vec2(1, 0),
    geom::Vec2(1, 1),
    geom::Vec2(0, 1),
});

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
    mLocalScene = rtcDeviceNewScene(device, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1 | RTC_INTERSECT8);
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
    tris[0] = TRIANGLES[0];
    tris[1] = TRIANGLES[1];
    rtcUnmapBuffer(mLocalScene, meshGeomId, RTC_INDEX_BUFFER);

    rtcCommit(mLocalScene);

    // Then register a user geometry so we can intersect against this mesh while giving ourselves
    // an opportunity to set its material.
    mGeomId = rtcNewUserGeometry3(scene, RTC_GEOMETRY_STATIC, 1);
    rtcSetUserData(scene, mGeomId, this);
    rtcSetBoundsFunction2(scene, mGeomId, boundsFunc, this);
    rtcSetIntersectFunction(scene, mGeomId, intersectFunc);
    rtcSetIntersectFunction8(scene, mGeomId, intersectFunc8);
    return std::vector<unsigned>( { mGeomId });
}

void
Rectangle::boundsFunc(void* userPtr,         /*!< pointer to user data */
                      void* geomUserPtr,     /*!< pointer to geometry user data */
                      size_t item,           /*!< item to calculate bounds for */
                      RTCBounds* bounds_o    /*!< returns calculated bounds */)
{
    // Assume we can dereference userPtr
    const auto rect = static_cast<Rectangle*>(userPtr);
    rtcGetBounds(rect->mLocalScene, *bounds_o);
}

void
Rectangle::intersectFunc(void* userPtr,   /*!< pointer to user data */
                         RTCRay& rtcRay,  /*!< ray to intersect */
                         size_t item      /*!< item to intersect */)
{
    // Assume we can dereference userPtr
    const auto rect = static_cast<Rectangle*>(userPtr);

    auto& ray = (Ray&)rtcRay;
    const auto geomID = ray.geomID;
    ray.geomID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect(rect->mLocalScene, rtcRay);

    if (ray.geomID != RTC_INVALID_GEOMETRY_ID) {
        ray.geomID = rect->mGeomId;
        ray.material = rect->material().get();
        ray.normal = -ray.normal;

        auto triangle = TRIANGLES[ray.primID];
        auto st0 = ST_VALUES[triangle.v0];
        auto st1 = ST_VALUES[triangle.v1];
        auto st2 = ST_VALUES[triangle.v2];

        const auto u = ray.uv.u(), v = ray.uv.v(), w = 1.0f-u-v;
        ray.uv = (st0 * w) + (st1 * u) + (st2 * v);
    } else {
        ray.geomID = geomID;
    }
}

void
Rectangle::intersectFunc8(const void* valid, /*!< pointer to valid mask */
                          void* userPtr,     /*!< pointer to user data */
                          RTCRay8& rtcRays,  /*!< ray packet to intersect */
                          size_t item        /*!< item to intersect */)
{
    // Assume we can dereference userPtr
    const auto rect = static_cast<Rectangle*>(userPtr);

    auto& rays = (Ray8&)rtcRays;
    auto geomID = rays.geomID;
    rays.geomID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect8(valid, rect->mLocalScene, rtcRays);

    for (auto i = 0; i < 8; ++i) {
        if (rays.geomID[i] != RTC_INVALID_GEOMETRY_ID) {
            rays.geomID[i] = rect->mGeomId;
            rays.material[i] = rect->material().get();
            rays.Ngx[i] = -rays.Ngx[i];
            rays.Ngy[i] = -rays.Ngy[i];
            rays.Ngz[i] = -rays.Ngz[i];

            const auto& triangle = TRIANGLES[rays.primID[i]];
            auto st0 = ST_VALUES[triangle.v0];
            auto st1 = ST_VALUES[triangle.v1];
            auto st2 = ST_VALUES[triangle.v2];

            const auto u = rays.u[i], v = rays.v[i], w = 1.0f-u-v;
            auto uv = (st0 * w) + (st1 * u) + (st2 * v);
            rays.u[i] = uv.u();
            rays.v[i] = uv.v();
        } else {
            rays.geomID[i] = geomID[i];
        }
    }
}

bool
Rectangle::bbox(float t0, float t1, geom::AABB& bbox) const
{
    RTCBounds bounds;
    rtcGetBounds(mLocalScene, bounds);
    auto min = geom::Vec3(bounds.lower_x, bounds.lower_y, bounds.lower_z);
    auto max = geom::Vec3(bounds.upper_x, bounds.upper_y, bounds.upper_z);
    bbox = geom::AABB(min, max);
    return true;
}

}
