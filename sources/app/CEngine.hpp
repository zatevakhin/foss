
#ifndef FOSS_CENGINE_HPP
#define FOSS_CENGINE_HPP

#include "core/window/CMainWindow.hpp"
#include "core/input/CInputEventManager.hpp"

#include "app/systems/C3DRenderSystem.hpp"
#include "app/systems/C2DRenderSystem.hpp"
#include "core/scene/CCamera.hpp"

#include "SEngineSettings.hpp"

#include <anax/anax.hpp>
#include <vector>

class CResourceLoader;


class CEngine
{
public:
    CEngine();
    ~CEngine();

    void initialize();

    void initializeVideo();

    void initializeInput();

    void run();
    void stop();

private:
    CMainWindow * mMainWindow;

    CChronometer * mChronometer;

    CInputEventManager * mInputManager;

    CResourceLoader * mResourceLoader;

    CCamera mCamera;

    anax::World mWorld;

    C2DRenderSystem * m2dRenderSystem;
    C3DRenderSystem * m3dRenderSystem;

    SEngineSettings mSettings;

    bool mIsRunning;
    bool mIsDebugMode;

private:
    void prepare();

    void loop();

    void finalize();

    void onEvent();

    void onUpdate(double delta);

    void onClear();

    void onDraw();

};

#endif //FOSS_CENGINE_HPP
