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

void
FlipNormals::transform(Ray8& rays) const
{
    for (auto i = 0; i < 8; ++i) {
        if (rays.geomID[i] != RTC_INVALID_GEOMETRY_ID) {
            rays.Ngx[i] = -rays.Ngx[i];
            rays.Ngy[i] = -rays.Ngy[i];
            rays.Ngz[i] = -rays.Ngz[i];
        }
    }
}

}
