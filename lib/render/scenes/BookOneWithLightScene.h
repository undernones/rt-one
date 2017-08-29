//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_BOOK_ONE_WITH_LIGHT_SCENE_H
#define RENDER_BOOK_ONE_WITH_LIGHT_SCENE_H

#include "Scene.h"

#include <vector>

namespace render
{

class BookOneWithLightScene : public Scene
{
public:
    BookOneWithLightScene(int width, int height);
    virtual ~BookOneWithLightScene() = default;
};

}

#endif // RENDER_BOOK_ONE_WITH_LIGHT_SCENE_H
