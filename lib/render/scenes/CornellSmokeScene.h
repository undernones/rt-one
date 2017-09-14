//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_CORNELL_SMOKE_SCENE_H
#define RENDER_CORNELL_SMOKE_SCENE_H

#include <render/Scene.h>

namespace render
{

class CornellSmokeScene : public Scene
{
public:
    CornellSmokeScene(int width, int height);
    virtual ~CornellSmokeScene() = default;
};

}

#endif // RENDER_CORNELL_SMOKE_SCENE_H
