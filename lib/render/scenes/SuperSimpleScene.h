//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_SUPER_SIMPLE_SCENE_H
#define RENDER_SUPER_SIMPLE_SCENE_H

#include <render/Scene.h>

namespace render
{

class SuperSimpleScene : public Scene
{
public:
    SuperSimpleScene(int width, int height);
    virtual ~SuperSimpleScene() = default;
};

}

#endif // RENDER_SUPER_SIMPLE_SCENE_H
