//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Mesh.h"

#include <geom/AABB.h>

namespace render
{

Mesh::Mesh(const std::shared_ptr<geom::TriangleMesh>& triMesh, std::shared_ptr<Material>& material)
    : Mesh(triMesh, std::move(material))
{
}

Mesh::Mesh(const std::shared_ptr<geom::TriangleMesh>& triMesh, std::shared_ptr<Material>&& material)
    : Renderable(material)
    , mLocalScene(nullptr)
    , mTriMesh(triMesh)
{
}

Mesh::~Mesh()
{
    if (mLocalScene != nullptr) {
        rtcDeleteScene(mLocalScene);
    }
}

bool
Mesh::bbox(float t0, float t1, geom::AABB& bbox) const
{
    RTCBounds bounds;
    rtcGetBounds(mLocalScene, bounds);
    auto min = geom::Vec3(bounds.lower_x, bounds.lower_y, bounds.lower_z);
    auto max = geom::Vec3(bounds.upper_x, bounds.upper_y, bounds.upper_z);
    bbox = geom::AABB(min, max);
    return true;
}

std::vector<unsigned>
Mesh::commit(RTCDevice device, RTCScene scene)
{
    if (mTriMesh == nullptr) {
        return std::vector<unsigned>();
    }

    // First register a new scene consisting of just the triangle mesh.
    mLocalScene = rtcDeviceNewScene(device, RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT | RTC_SCENE_HIGH_QUALITY, RTC_INTERSECT1);
    auto faceCount = mTriMesh->triangles().size();
    auto vertCount = mTriMesh->verts().size();
    auto meshGeomId = rtcNewTriangleMesh(mLocalScene, RTC_GEOMETRY_STATIC, faceCount, vertCount);

    rtcSetBuffer2(mLocalScene, meshGeomId, RTC_VERTEX_BUFFER, mTriMesh->verts().data(), 0, sizeof(geom::Vec3));
    rtcSetBuffer2(mLocalScene, meshGeomId, RTC_INDEX_BUFFER, mTriMesh->triangles().data(), 0, sizeof(geom::Triangle));

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
Mesh::boundsFunc(void* userPtr,         /*!< pointer to user data */
                 void* geomUserPtr,     /*!< pointer to geometry user data */
                 size_t item,           /*!< item to calculate bounds for */
                 RTCBounds* bounds_o    /*!< returns calculated bounds */)
{
    // Assume we can dereference userPtr
    const auto mesh = static_cast<Mesh*>(userPtr);
    rtcGetBounds(mesh->mLocalScene, *bounds_o);
}

void
Mesh::intersectFunc(void* userPtr,   /*!< pointer to user data */
                    RTCRay& rtcRay,  /*!< ray to intersect */
                    size_t item      /*!< item to intersect */)
{
    // Assume we can dereference userPtr
    const auto mesh = static_cast<Mesh*>(userPtr);

    auto& ray = (Ray&)rtcRay;
    const auto geomID = ray.geomID;
    ray.geomID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect(mesh->mLocalScene, rtcRay);

    if (ray.geomID != RTC_INVALID_GEOMETRY_ID) {
        ray.geomID = mesh->mGeomId;
        ray.material = mesh->material().get();

        const auto u = ray.uv.u(), v = ray.uv.v(), w = 1.0f-u-v;
        const auto& triangle = mesh->triangles()[ray.primID];

        const auto& normals = mesh->normals();
        if (normals.size() > 0) {
            auto n0 = normals[triangle.v0];
            auto n1 = normals[triangle.v1];
            auto n2 = normals[triangle.v2];
            ray.normal = (n0 * w) + (n1 * u) + (n2 * v);
        } else {
            ray.normal = -ray.normal.normalized();
        }

        const auto& uvs = mesh->uvs();
        if (uvs.size() > 0) {
            auto uv0 = uvs[triangle.v0];
            auto uv1 = uvs[triangle.v1];
            auto uv2 = uvs[triangle.v2];
            ray.uv = (uv0 * w) + (uv1 * u) + (uv2 * v);
        }
    } else {
        ray.geomID = geomID;
    }
}

}
