//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "FlipNormals.h"

namespace render
{

FlipNormals::FlipNormals(std::shared_ptr<Hitable>& object)
    : FlipNormals(std::move(object))
{
}

FlipNormals::FlipNormals(std::shared_ptr<Hitable>&& object)
    : Transform(object)
{
}

void
FlipNormals::transform(Ray& ray)
{
    ray.normal = -ray.normal;
}

}
