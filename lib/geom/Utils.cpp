//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Utils.h"

namespace geom
{

float
toRadians(float degrees)
{
    return degrees * M_PI / 180;
}

float
toDegrees(float radians)
{
    return radians * 180 / M_PI;
}

}
