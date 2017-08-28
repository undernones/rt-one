//
// Copyright © 2017 Undernones. All rights reserved.
//

// Self
#include "CApp.h"

// System
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <tbb/tbb.h>

// One
#include <geom/Vec3.h>
#include <geom/Ray.h>
#include <render/AstronomyScene.h>
#include <render/BookOneScene.h>
#include <render/Renderer.h>

#if DEBUG
#define PARALLEL 0
#else
#define PARALLEL 1
#endif

namespace
{

#define BIG 0
#if BIG
const auto nx = 1680;
const auto ny = 1050;
#else
const auto nx = 600;
const auto ny = 300;
#endif

void
showImage(const render::Image& img,
          CApp::Channel channel,
          SDL_Renderer* renderer,
          SDL_Texture* texture)
{
    std::vector<uint32_t> buffer;

    switch (channel) {
        case CApp::Channel::ALL:
            buffer = img.getRgba();
            break;

            case CApp::Channel::RED:
            buffer = img.rToRgba();
            break;

            case CApp::Channel::GREEN:
            buffer = img.gToRgba();
            break;

            case CApp::Channel::BLUE:
            buffer = img.bToRgba();
            break;
        }

    SDL_UpdateTexture(texture, NULL, &buffer[0], img.cols() * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

}

CApp::CApp(int argc, const char* argv[])
    : mIsRunning(true)
    , mNeedsToClear(true)
    , mIsDragging(false)
    , mSampleCount(0)
    , mChannel(Channel::ALL)
{
}

int
CApp::OnExecute()
{
    if (!OnInit()) {
        return -1;
    }

    auto event = SDL_Event();

    while (mIsRunning) {
        while (SDL_PollEvent(&event)) {
            OnEvent(event);
        }

        OnLoop();
        OnRender();
    }

    OnCleanup();
    return 0;
}

bool
CApp::OnInit()
{
    mScene = std::make_unique<render::AstronomyScene>(nx, ny);

    // Verify the validity of the camera.
    auto camera = mScene->camera();
    if (camera.width() <= 0 || camera.height() <= 0) {
        std::cerr << "Bad camera dimensions: (" << camera.width() << ", " << camera.height() << ")" << std::endl;
        return false;
    }

    // Allocate the image.
    mImage = render::Image(camera.height(), camera.width());
    
    // Set up the SDL window.
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    SDL_CreateWindowAndRenderer(nx,
                                ny,
                                SDL_WINDOW_OPENGL,
                                &mWindow,
                                &mRenderer);

    if (!mWindow || !mRenderer) {
        return false;
    }

    auto stream = std::stringstream();
    stream << "one - " << camera.width() << " x " << camera.height();
    SDL_SetWindowTitle(mWindow, stream.str().c_str());

    mTexture = SDL_CreateTexture(mRenderer,
                                 SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_TARGET,
                                 nx,
                                 ny);

    return mTexture != nullptr;
}

void
CApp::OnEvent(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_QUIT:
            mIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_1:
                    mChannel = Channel::ALL;
                    break;

                case SDLK_2:
                    mChannel = Channel::RED;

                case SDLK_3:
                    mChannel = Channel::GREEN;

                case SDLK_4:
                    mChannel = Channel::BLUE;

                default:
                    ; // Do nothing
            }
            showImage(mImage, mChannel, mRenderer, mTexture);
            break;

        case SDL_MOUSEBUTTONUP:
            OnMouseButtonUp(event.button);
            break;

        case SDL_MOUSEMOTION:
            OnMouseMotion(event.motion);
            break;

        case SDL_MOUSEWHEEL:
            OnMouseWheel(event.wheel);
            break;

        case SDL_USEREVENT:
            showImage(mImage, mChannel, mRenderer, mTexture);
            break;
    }
}

void
CApp::OnMouseButtonUp(const SDL_MouseButtonEvent& event)
{
    if (!mIsDragging) {
        // The user clicked without dragging. Might be useful to trigger a render of a single pixel.
    }
    mIsDragging = false;
}

void
CApp::OnMouseMotion(const SDL_MouseMotionEvent& event)
{
    if (event.state & SDL_BUTTON_LMASK) {
        // TODO: rotate
    }
}

void
CApp::OnMouseWheel(const SDL_MouseWheelEvent& event)
{
    // TODO: something cool.
}

void
CApp::OnLoop()
{
}

void
CApp::OnRender()
{
    if (mNeedsToClear) {
        mImage.clear();
        mNeedsToClear = false;
        mSampleCount = 0;
    }

    static auto totalTimeMs = 0.0;
    static auto totalPasses = 0;

    mSampleCount++;

    auto start = std::chrono::steady_clock::now();

    auto camera = mScene->camera();

#if PARALLEL
    tbb::parallel_for(int(0), mImage.rows(), int(1), [&](int row) {
#else
    for (auto row = 0; row < mImage.rows(); ++row) {
#endif
        for (auto col = 0; col < mImage.cols(); ++col) {
            const auto s = (col + drand48()) / mImage.cols();
            const auto t = (row + drand48()) / mImage.rows();
            const auto ray = camera.getRay(s, t);

            // Keep a running average of samples.
            const auto sample = render::Renderer::color(ray, *mScene);
            const auto current = mImage.value(row, col);
            const auto newValue = current + (sample - current) / mSampleCount;

            mImage.setValue(row, col, newValue);
        }
#if PARALLEL
    });
#else
    }
#endif

    auto end = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration<double, std::milli>(end - start).count();
    totalTimeMs += diff;
    auto fps = ++totalPasses / (totalTimeMs / 1000);
    std::cout << diff << " ms,\t" << fps << " FPS\t" << totalPasses << " samples" << std::endl;

    showImage(mImage, mChannel, mRenderer, mTexture);
}

void
CApp::OnCleanup()
{
    SDL_Quit();
}
