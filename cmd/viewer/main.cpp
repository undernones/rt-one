//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "CApp.h"

#include <xmmintrin.h>
#include <pmmintrin.h>
#include <iostream>

int
main(int argc, const char * argv[])
{
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

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
