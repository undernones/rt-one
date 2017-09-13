//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "FlipNormals.h"

namespace render
{

FlipNormals::FlipNormals(std::shared_ptr<Renderable>& object)
    : FlipNormals(std::move(object))
{
}

FlipNormals::FlipNormals(std::shared_ptr<Renderable>&& object)
    : Transform(object)
{
}

void
FlipNormals::transform(Ray& ray) const
{
    ray.normal = -ray.normal;
}

}
