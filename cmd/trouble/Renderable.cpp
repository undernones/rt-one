//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Renderable.h"

Renderable::Renderable(std::shared_ptr<Material>& material)
    : mGeomId(RTC_INVALID_GEOMETRY_ID)
    , mMaterial(material)
{
}

Renderable::Renderable(std::shared_ptr<Material>&& material)
    : Renderable(material)
{
}
