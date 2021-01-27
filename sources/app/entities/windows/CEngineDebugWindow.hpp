#pragma once

#include "IWindow.hpp"
#include "app/scene/CFreeCamera.hpp"

class CEngineDebugWindow : public IWindow
{
public:
    CEngineDebugWindow(const TFreeCameraPtr& camera);
    ~CEngineDebugWindow();

    void draw() override;

private:
    TFreeCameraPtr mCamera;
};
