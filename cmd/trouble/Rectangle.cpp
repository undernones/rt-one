//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Rectangle.h"

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
    const auto rect = static_cast<RectYZ*>(userPtr);

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

RectYZ::RectYZ(Plane plane, float ymin, float ymax, float zmin, float zmax, float offset, std::shared_ptr<Material>& material)
    : RectYZ(plane, ymin, ymax, zmin, zmax, offset, std::move(material))
{
}

RectYZ::RectYZ(Plane plane, float ymin, float ymax, float zmin, float zmax, float offset, std::shared_ptr<Material>&& material)
    : Renderable(material)
    , mPlane(plane)
    , ymin(ymin)
    , ymax(ymax)
    , zmin(zmin)
    , zmax(zmax)
    , mOffset(offset)
{
}

bool
RectYZ::bbox(float t0, float t1, geom::AABB& bbox) const
{
    auto min = geom::Vec3(mOffset - EPSILON, ymin, zmin);
    auto max = geom::Vec3(mOffset + EPSILON, ymax, zmax);
    bbox = geom::AABB(min, max);
    return true;
}

std::vector<unsigned>
RectYZ::commit(RTCDevice device, RTCScene scene)
{
    mLocalScene = rtcDeviceNewScene(device, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
    auto meshId = rtcNewTriangleMesh(mLocalScene, RTC_GEOMETRY_STATIC, 2, 4);

    auto verts = (Vertex*)rtcMapBuffer(mLocalScene, meshId, RTC_VERTEX_BUFFER);
    switch (mPlane) {
        case Plane::XY:
            break;

        case Plane::XZ:
            break;

        case Plane::YZ:
            verts[0] = { mOffset, ymin, zmin, 1 };
            verts[1] = { mOffset, ymax, zmin, 1 };
            verts[2] = { mOffset, ymax, zmax, 1 };
            verts[3] = { mOffset, ymin, zmax, 1 };
            break;
    }
    rtcUnmapBuffer(mLocalScene, meshId, RTC_VERTEX_BUFFER);

    auto tris = (Triangle*)rtcMapBuffer(mLocalScene, meshId, RTC_INDEX_BUFFER);
    tris[0] = { 0, 1, 3 };
    tris[1] = { 1, 2, 3 };
    rtcUnmapBuffer(mLocalScene, meshId, RTC_INDEX_BUFFER);

    rtcCommit(mLocalScene);

    // Then register a user geometry so we can intersect against this mesh while giving ourselves
    // an opportunity to do custom stuff to the ray before returning.
    mGeomId = rtcNewUserGeometry3(scene, RTC_GEOMETRY_STATIC, 1);
    rtcSetUserData(scene, mGeomId, this);
    rtcSetBoundsFunction2(scene, mGeomId, boundsFunc, this);
    rtcSetIntersectFunction(scene, mGeomId, intersectFunc);
    return std::vector<unsigned>({ mGeomId });
}

void
RectYZ::intersectFunc(void* userPtr,   /*!< pointer to user data */
                      RTCRay& rtcRay,  /*!< ray to intersect */
                      size_t item      /*!< item to intersect */)
{
    // Assume we can dereference userPtr
    const auto rect = static_cast<RectYZ*>(userPtr);

    const auto geomID = rtcRay.geomID;
    rtcRay.geomID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect(rect->mLocalScene, rtcRay);

    if (rtcRay.geomID != RTC_INVALID_GEOMETRY_ID) {
        rtcRay.geomID = rect->geomId();
        auto& ray = (Ray&)rtcRay;
        ray.material = rect->material().get();
        ray.normal = -ray.normal.normalized();
    } else {
        rtcRay.geomID = geomID;
    }
}
