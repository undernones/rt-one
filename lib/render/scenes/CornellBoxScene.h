//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_CORNELL_BOX_SCENE_H
#define RENDER_CORNELL_BOX_SCENE_H

#include <render/Scene.h>

namespace render
{

class CornellBoxScene : public Scene
{
public:
    CornellBoxScene(int width, int height);
    virtual ~CornellBoxScene() = default;
};

}

#endif // RENDER_CORNELL_BOX_SCENE_H
