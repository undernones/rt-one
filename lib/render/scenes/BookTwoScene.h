//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_BOOK_TWO_SCENE_H
#define RENDER_BOOK_TWO_SCENE_H

#include <render/Scene.h>

namespace render
{

class BookTwoScene : public Scene
{
public:
    BookTwoScene(int width, int height);
    virtual ~BookTwoScene() = default;
};
    
}

#endif // RENDER_BOOK_TWO_SCENE_H
