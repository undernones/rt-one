//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_RECT_LIGHT_SCENE_H
#define RENDER_RECT_LIGHT_SCENE_H

#include <render/Scene.h>

namespace render
{

class RectLightScene : public Scene
{
public:
    RectLightScene(int width, int height);
    virtual ~RectLightScene() = default;
};

}

#endif // RENDER_RECT_LIGHT_SCENE_H
