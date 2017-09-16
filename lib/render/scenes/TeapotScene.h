//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_TEAPOT_SCENE_H
#define RENDER_TEAPOT_SCENE_H

#include <render/Scene.h>

namespace render
{

class TeapotScene : public Scene
{
public:
    TeapotScene(int width, int height);
    virtual ~TeapotScene() = default;
};

}

#endif // RENDER_TEAPOT_SCENE_H
