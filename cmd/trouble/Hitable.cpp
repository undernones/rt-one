//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Hitable.h"

Hitable::Hitable(std::shared_ptr<Material>& material)
    : mGeomId(RTC_INVALID_GEOMETRY_ID)
    , mMaterial(material)
{
}

Hitable::Hitable(std::shared_ptr<Material>&& material)
    : Hitable(material)
{
}
