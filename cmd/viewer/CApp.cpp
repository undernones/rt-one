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
#include <unistd.h>

// One
#include <geom/Utils.h>
#include <geom/Vec3.h>
#include <render/AstronomyScene.h>
#include <render/BookOneScene.h>
#include <render/BookOneWithLightScene.h>
#include <render/BookTwoScene.h>
#include <render/CornellBoxScene.h>
#include <render/CornellSmokeScene.h>
#include <render/Ray.h>
#include <render/RectLightScene.h>
#include <render/Renderer.h>
#include <render/SuperSimpleScene.h>
#include <render/TeapotScene.h>

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
const auto nx = 640;
const auto ny = 400;
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
    , mIsPaused(false)
    , mNeedsToClear(true)
    , mIsDragging(false)
    , mSampleCount(0)
    , mChannel(Channel::ALL)
{
#if PARALLEL
    std::cout << "Multi-threaded" << std::endl;
#else
    std::cout << "Single threaded" << std::endl;
#endif
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
    mScene = std::make_unique<render::TeapotScene>(nx, ny);
    mUpVector = mScene->camera().up();

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
                    break;

                case SDLK_3:
                    mChannel = Channel::GREEN;
                    break;

                case SDLK_4:
                    mChannel = Channel::BLUE;
                    break;

                case SDLK_p:
                    mIsPaused = !mIsPaused;
                    break;

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
        auto col = event.x;
        auto row = mImage.rows() - event.y;
        auto r = geom::rand2();
        const auto s = (col + r[0]) / mImage.cols();
        const auto t = (row + r[1]) / mImage.rows();
        const auto& camera = mScene->camera();
        auto ray = camera.getRay(s, t);
        const auto sample = render::Renderer::trace(ray, *mScene);
        std::cout << "sample: [" << sample.r() << ", " << sample.g() << ", " << sample.b() << "]" << std::endl;
    }
    mIsDragging = false;
}

void
CApp::OnMouseMotion(const SDL_MouseMotionEvent& event)
{
    if (event.state & SDL_BUTTON_LMASK) {
        mIsDragging = true;

        // In order to support tumbling, we make a copy of the old camera with a new position.
        // We compute the new position by converting the old cartesian coordinates to spherical
        // coordinates, add radian offsets derived from the mouse movement, and convert back to
        // cartesian.
        const auto& oldCamera = mScene->camera();

        // Compute the offset from the mouse movement, constraining the rotation depending on the
        // keyboard state.
        auto offsetX = geom::toRadians(-event.xrel);
        auto offsetY = geom::toRadians(-event.yrel);

        const auto state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_X] != 0 && state[SDL_SCANCODE_Y] == 0) {
            offsetY = 0.f;
        } else if (state[SDL_SCANCODE_Y] != 0 && state[SDL_SCANCODE_X] == 0) {
            offsetX = 0.f;
        }
        auto sphericalOffset = geom::Vec3(0, offsetX, offsetY);

        // This is the vector we will be rotating
        auto lookToCamera = oldCamera.position() - oldCamera.lookAt();

        // Do the coordinate space conversions with the offset
        auto spherical = geom::cartesianToSpherical(lookToCamera);
        auto newSpherical = spherical + sphericalOffset;
        auto newLookToCamera = geom::sphericalToCartesian(newSpherical);
        auto newPosition = oldCamera.lookAt() + newLookToCamera;

        // Here we make the copy with the new position, and we maintain the original up vector.
        auto newCamera = render::Camera(newPosition,
                                        oldCamera.lookAt(),
                                        mUpVector,
                                        oldCamera.fov(),
                                        oldCamera.width(),
                                        oldCamera.height(),
                                        oldCamera.aperture(),
                                        oldCamera.focusDistance(),
                                        oldCamera.time0(),
                                        oldCamera.time1());
        mScene->setCamera(newCamera);
        mNeedsToClear = true;
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
    if (mIsPaused) {
        usleep(100);
        return;
    }

    if (mNeedsToClear) {
        mImage.clear();
        mNeedsToClear = false;
        mSampleCount = 0;
    }

    static auto totalTimeMs = 0.0;
    static auto totalPasses = 0;

    mSampleCount++;

    auto start = std::chrono::steady_clock::now();

    const auto& camera = mScene->camera();
    const auto colsInv = 1.f / mImage.cols();
    const auto rowsInv = 1.f / mImage.rows();

#if PARALLEL
    tbb::parallel_for(int(0), mImage.rows(), int(1), [&](int row) {
#else
    for (auto row = 0; row < mImage.rows(); ++row) {
#endif
        auto s = std::array<float, 8>();
        auto t = std::array<float, 8>();

        for (auto col = 0; col < mImage.cols(); col += 8) {
            auto valid = std::array<int32_t, 8>({-1,-1,-1,-1,-1,-1,-1,-1});
            auto rands = geom::rand8();
            auto randt = geom::rand8();
            for (auto i = 0; i < 8; ++i) {
                if (col < mImage.cols()) {
                    s[i] = (col + i + rands[i]) * colsInv;
                    t[i] = (row + 0 + randt[i]) * rowsInv;
                } else {
                    valid[i] = 0;
                }
            }
            auto ray = camera.getRays(s, t);

            // Keep a running average of samples.
            const auto samples = render::Renderer::trace(valid, ray, *mScene);
            for (auto i = 0; i < 8 && (col + i) < mImage.cols(); ++i) {
                const auto current = mImage.value(row, col + i);
                const auto newValue = current + (samples[i] - current) / mSampleCount;
                mImage.setValue(row, col + i, newValue);
            }
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
    std::cout << diff << " ms,\t" << fps << " FPS\t" << mSampleCount << " samples" << std::endl;

    showImage(mImage, mChannel, mRenderer, mTexture);
}

void
CApp::OnCleanup()
{
    SDL_Quit();
}
