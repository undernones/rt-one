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

RectYZ::RectYZ(float x, float ymin, float ymax, float zmin, float zmax)
    : x(x)
    , ymin(ymin)
    , ymax(ymax)
    , zmin(zmin)
    , zmax(zmax)
    , geomID(RTC_INVALID_GEOMETRY_ID)
{
}

bool
RectYZ::bbox(float t0, float t1, geom::AABB& bbox) const
{
    auto min = geom::Vec3(x - EPSILON, ymin, zmin);
    auto max = geom::Vec3(x + EPSILON, ymax, zmax);
    bbox = geom::AABB(min, max);
    return true;
}

unsigned
RectYZ::commit(RTCDevice device, RTCScene scene)
{
    localScene = rtcDeviceNewScene(device, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
    auto meshId = rtcNewTriangleMesh(localScene, RTC_GEOMETRY_STATIC, 2, 4);

    auto verts = (Vertex*)rtcMapBuffer(localScene, meshId, RTC_VERTEX_BUFFER);
    verts[0] = { x, ymin, zmin, 1 };
    verts[1] = { x, ymax, zmin, 1 };
    verts[2] = { x, ymax, zmax, 1 };
    verts[3] = { x, ymin, zmax, 1 };
    rtcUnmapBuffer(localScene, meshId, RTC_VERTEX_BUFFER);

    auto tris = (Triangle*)rtcMapBuffer(localScene, meshId, RTC_INDEX_BUFFER);
    tris[0] = { 0, 1, 3 };
    tris[1] = { 1, 2, 3 };
    rtcUnmapBuffer(localScene, meshId, RTC_INDEX_BUFFER);

    rtcCommit(localScene);

    // Then register a user geometry so we can intersect against this mesh while giving ourselves
    // an opportunity to do custom stuff to the ray before returning.
    geomID = rtcNewUserGeometry3(scene, RTC_GEOMETRY_STATIC, 1);
    rtcSetUserData(scene, geomID, this);
    rtcSetBoundsFunction2(scene, geomID, boundsFunc, this);
    rtcSetIntersectFunction(scene, geomID, intersectFunc);

    return geomID;
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

    rtcIntersect(rect->localScene, rtcRay);

    if (rtcRay.geomID != RTC_INVALID_GEOMETRY_ID) {
        rtcRay.geomID = rect->geomID;
        auto& ray = (Ray&)rtcRay;
        ray.normal = -ray.normal.normalized();
    } else {
        rtcRay.geomID = geomID;
    }
}
