//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef VIEWER_CAPP_H
#define VIEWER_CAPP_H

// System
#include <SDL2/SDL.h>
#include <string>

// One
#include <render/Image.h>
#include <render/Renderer.h>

class CApp
{
public:
    CApp(int argc, const char* argv[]);

    int OnExecute();
    bool OnInit();
    void OnEvent(const SDL_Event& event);
    void OnMouseButtonUp(const SDL_MouseButtonEvent& event);
    void OnMouseMotion(const SDL_MouseMotionEvent& event);
    void OnMouseWheel(const SDL_MouseWheelEvent& event);
    void OnLoop();
    void OnRender();
    void OnCleanup();

    enum class Channel
    {
        ALL,
        RED,
        GREEN,
        BLUE,
    };

private:
    bool mIsRunning;
    bool mNeedsToClear;
    bool mIsDragging;
    int mSampleCount;

    struct SDLVars
    {
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;

        SDLVars() : window(nullptr), renderer(nullptr), texture(nullptr) {}
    };
    SDLVars mSDLVars;

    Channel mChannel;

    render::Renderer mRenderer;
    render::Image mImage;
};

#endif // VIEWER_CAPP_H
