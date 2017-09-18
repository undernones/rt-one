//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <geom/TriangleMesh.h>
#include <render/Renderable.h>

namespace  render
{

class Mesh : public Renderable
{
public:
    Mesh(const std::shared_ptr<geom::TriangleMesh>& triMesh, std::shared_ptr<Material>& material);
    Mesh(const std::shared_ptr<geom::TriangleMesh>& triMesh, std::shared_ptr<Material>&& material);
    virtual ~Mesh();

    virtual bool bbox(float t0, float t1, geom::AABB& bbox) const;
    virtual std::vector<unsigned> commit(RTCDevice device, RTCScene scene);

private:
    static void boundsFunc(void* userPtr,         /*!< pointer to user data */
                           void* geomUserPtr,     /*!< pointer to geometry user data */
                           size_t item,           /*!< item to calculate bounds for */
                           RTCBounds* bounds_o    /*!< returns calculated bounds */);

    static void intersectFunc(void* userPtr,   /*!< pointer to user data */
                              RTCRay& rtcRay,  /*!< ray to intersect */
                              size_t item      /*!< item to intersect */);

    const std::vector<geom::Vec3>& verts() const { return mTriMesh->verts(); }
    const std::vector<geom::Triangle>& triangles() const { return mTriMesh->triangles(); }
    const std::vector<geom::Vec3>& normals() const { return mTriMesh->normals(); }
    const std::vector<geom::Vec2>& uvs() const { return mTriMesh->uvs(); }

    RTCScene mLocalScene;
    std::shared_ptr<geom::TriangleMesh> mTriMesh;
};

}

#endif // RENDER_MESH_H
