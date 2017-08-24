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

// One
#include <geom/Vec3.h>
#include <geom/Ray.h>
#include <render/Camera.h>
#include <render/Renderer.h>

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
    // Allocate the image.
    mImage = render::Image(ny, nx);
    
    // Set up the SDL window.
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    SDL_CreateWindowAndRenderer(nx,
                                ny,
                                SDL_WINDOW_OPENGL,
                                &mSDLVars.window,
                                &mSDLVars.renderer);

    if (!mSDLVars.window || !mSDLVars.renderer) {
        return false;
    }

    auto stream = std::stringstream();
    stream << "one - " << nx << " x " << ny;
    SDL_SetWindowTitle(mSDLVars.window, stream.str().c_str());

    mSDLVars.texture = SDL_CreateTexture(mSDLVars.renderer,
                                         SDL_PIXELFORMAT_RGBA8888,
                                         SDL_TEXTUREACCESS_TARGET,
                                         nx,
                                         ny);

    return mSDLVars.texture != nullptr;
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
            showImage(mImage, mChannel, mSDLVars.renderer, mSDLVars.texture);
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
            showImage(mImage, mChannel, mSDLVars.renderer, mSDLVars.texture);
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

    auto lowerLeft = geom::Vec3(-2, -1, -1);
    auto horizontal = geom::Vec3(4, 0, 0);
    auto vertical = geom::Vec3(0, 2, 0);
    auto origin = geom::Vec3(0, 0, 0);
    auto camera = render::Camera(origin, lowerLeft, horizontal, vertical);

    for (auto row = 0; row < mImage.rows(); ++row) {
        for (auto col = 0; col < mImage.cols(); ++col) {
            const auto s = (col + drand48()) / mImage.cols();
            const auto t = (row + drand48()) / mImage.rows();
            const auto ray = camera.getRay(s, t);

            // Keep a running average of samples.
            const auto sample = mRenderer.color(ray);
            const auto current = mImage.value(row, col);
            const auto newValue = current + (sample - current) / mSampleCount;

            mImage.setValue(row, col, newValue);
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration<double, std::milli>(end - start).count();
    totalTimeMs += diff;
    auto fps = ++totalPasses / (totalTimeMs / 1000);
    std::cout << diff << " ms,\t" << fps << " FPS\t" << totalPasses << " samples" << std::endl;

    showImage(mImage, mChannel, mSDLVars.renderer, mSDLVars.texture);
}

void
CApp::OnCleanup()
{
    SDL_Quit();
}
