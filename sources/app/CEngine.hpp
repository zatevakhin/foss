
#ifndef FOSS_CENGINE_HPP
#define FOSS_CENGINE_HPP

#include "core/window/CMainWindow.hpp"
#include "core/input/CInputEventManager.hpp"

#include "app/systems/C3DRenderSystem.hpp"
#include "app/systems/C2DRenderSystem.hpp"
#include "core/scene/CCamera.hpp"

#include "SEngineSettings.hpp"

#include <anax/anax.hpp>
#include <memory>
#include <vector>

class CResourceLoader;


class CEngine
{
public:
    CEngine();
    ~CEngine();

    void run();
    void stop();

private:
    void initialize();
    void initializeVideo();
    void initializeInput();

    void prepare();

    void loop();

    void finalize();

    void onEvent();

    void onUpdate(double delta);

    void onClear();

    void onDraw();
    void onSwapBuffers();

private:
    CChronometer mChronometer;

    CCamera mCamera;

    anax::World mWorld;

    SEngineSettings mSettings;

    std::unique_ptr<CMainWindow> mMainWindow;
    std::unique_ptr<CInputEventManager> mInputManager;
    std::unique_ptr<CResourceLoader> mResourceLoader;
    std::unique_ptr<C2DRenderSystem> m2dRenderSystem;
    std::unique_ptr<C3DRenderSystem> m3dRenderSystem;


    bool mIsRunning;
    bool mIsDebugMode;
};

#endif //FOSS_CENGINE_HPP
