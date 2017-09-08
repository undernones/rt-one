//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_BOOK_ONE_SCENE_H
#define RENDER_BOOK_ONE_SCENE_H

#include <render/Scene.h>

namespace render
{

class BookOneScene : public Scene
{
public:
    BookOneScene(int width, int height);
    virtual ~BookOneScene() = default;
};

}

#endif // RENDER_BOOK_ONE_SCENE_H
