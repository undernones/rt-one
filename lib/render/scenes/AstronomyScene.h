//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_ASTRONOMY_SCENE_H
#define RENDER_ASTRONOMY_SCENE_H

#include "Scene.h"

namespace render
{

class AstronomyScene : public Scene
{
public:
    AstronomyScene(int width, int height);
    virtual ~AstronomyScene() = default;
};

}

#endif // RENDER_ASTRONOMY_SCENE_H
