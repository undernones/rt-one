//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef TROUBLE_RECT_LIGHT_SCENE_H
#define TROUBLE_RECT_LIGHT_SCENE_H

#include "Scene.h"

// TODO: move these to the cpp file
#include "Rectangle.h"
#include "Sphere.h"

class RectLightScene : public Scene
{
public:
    RectLightScene(int width, int height);
    virtual ~RectLightScene() = default;

private:
    std::shared_ptr<Sphere> sphere1;
    std::shared_ptr<Sphere> sphere2;
    std::shared_ptr<Sphere> sphere3;
    std::shared_ptr<RectYZ> rect;
};

#endif // TROUBLE_RECT_LIGHT_SCENE_H
