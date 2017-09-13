//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Box.h"

#include "FlipNormals.h"
#include "RenderableList.h"
#include "Rectangle.h"

using namespace std;

namespace render
{

Box::Box(const geom::Vec3& min, const geom::Vec3& max, std::shared_ptr<Material>& material)
    : Box(min, max, move(material))
{
}

Box::Box(const geom::Vec3& min, const geom::Vec3& max, shared_ptr<Material>&& material)
    : Renderable(material)
    , mMin(min)
    , mMax(max)
{
    auto list = vector<shared_ptr<Renderable>>();

    // Bottom
    list.emplace_back(make_shared<FlipNormals>(make_shared<Rectangle>(Plane::XZ, min.x(), max.x(), min.z(), max.z(), min.y(), material)));

    // Left
    list.emplace_back(make_shared<FlipNormals>(make_shared<Rectangle>(Plane::YZ, min.y(), max.y(), min.z(), max.z(), min.x(), material)));

    // Front
    list.emplace_back(make_shared<Rectangle>(Plane::XY, min.x(), max.x(), min.y(), max.y(), max.z(), material));

    // Right
    list.emplace_back(make_shared<Rectangle>(Plane::YZ, min.y(), max.y(), min.z(), max.z(), max.x(), material));

    // Back
    list.emplace_back(make_shared<FlipNormals>(make_shared<Rectangle>(Plane::XY, min.x(), max.x(), min.y(), max.y(), min.z(), material)));

    // Top
    list.emplace_back(make_shared<Rectangle>(Plane::XZ, min.x(), max.x(), min.z(), max.z(), max.y(), material));

    mChildren = make_shared<RenderableList>(list);
}

bool
Box::bbox(float t0, float t1, geom::AABB& bbox) const
{
    return mChildren->bbox(t0, t1, bbox);
}
    
std::vector<unsigned>
Box::commit(RTCDevice device, RTCScene scene)
{
    return mChildren->commit(device, scene);
}

}
