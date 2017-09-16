//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef GEOM_TRIANGLE_MESH_H
#define GEOM_TRIANGLE_MESH_H

#include <geom/Vec2.h>
#include <geom/Vec3.h>
#include <iostream>
#include <string>
#include <vector>

namespace geom
{

struct Triangle { int v0, v1, v2; };

class TriangleMesh
{
public:
    TriangleMesh() = default;
    TriangleMesh(const TriangleMesh& other) = default;
    TriangleMesh(const std::string& fileName);
    TriangleMesh(std::istream& stream);

    ~TriangleMesh() = default;

    const std::vector<geom::Vec3>& verts() const { return mVerts; }
    const std::vector<Triangle>& triangles() const { return mTriangles; }
    const std::vector<Vec3>& normals() const { return mNormals; }
    const std::vector<Vec2>& uvs() const { return mUvs; }

private:
    void load(std::istream& stream);

    std::vector<geom::Vec3> mVerts;
    std::vector<Triangle> mTriangles;
    std::vector<Vec3> mNormals;
    std::vector<Vec2> mUvs;
};

}

#endif // GEOM_TRIANGLE_MESH_H
