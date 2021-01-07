#pragma once

#include "app/window/CMainWindow.hpp"
#include "app/input/CInputEventManager.hpp"

#include "app/systems/CRotationUpdateSystem.hpp"
#include "app/systems/C3DRenderSystem.hpp"
#include "app/systems/C2DRenderSystem.hpp"
#include "app/systems/CCullingSystem.hpp"
#include "app/systems/CPickingSystem.hpp"
#include "app/scene/CCamera.hpp"
#include "app/resources/CShaderManager.hpp"

#include "SEngineSettings.hpp"

#include "ecs/EntityManager.hpp"
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

    ecs::EntityManager mEntityManager;

    SEngineSettings mSettings;

    std::unique_ptr<CMainWindow> mMainWindow;
    std::unique_ptr<CInputEventManager> mInputManager;
    std::unique_ptr<CResourceLoader> mResourceLoader;

    std::unique_ptr<C2DRenderSystem> m2dRenderSystem;
    std::unique_ptr<C3DRenderSystem> m3dRenderSystem;

    std::unique_ptr<CRotationUpdateSystem> mRotationUpdateSystem;
    std::unique_ptr<CCullingSystem> mCullingSystem;
    std::unique_ptr<CPickingSystem> mPickingSystem;
    std::unique_ptr<CShaderManager> mShaderManager;

    bool mIsRunning;
    bool mIsDebugMode;
};
