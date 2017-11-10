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
#include <render/Scene.h>

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
    void OnSave();
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
    bool mIsPaused;
    bool mNeedsToClear;
    bool mIsDragging;
    int mSampleCount;

    Channel mChannel;

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    SDL_Texture* mTexture;

    geom::Vec3 mUpVector;
    std::unique_ptr<render::Scene> mScene;
    render::Image mImage;
};

#endif // VIEWER_CAPP_H
