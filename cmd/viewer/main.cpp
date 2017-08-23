//
// Copyright © 2017 Undernones. All rights reserved.
//

#include <iostream>

#include "CApp.h"

int
main(int argc, const char * argv[])
{
    try {
        auto app = CApp(argc, argv);

        auto event = SDL_Event();
        event.type = SDL_USEREVENT;
        SDL_PushEvent(&event);

        return app.OnExecute();

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
}
