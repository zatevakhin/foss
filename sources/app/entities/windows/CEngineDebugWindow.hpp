#pragma once

#include "IWindow.hpp"


class CCamera;

class CEngineDebugWindow : public IWindow
{
public:
    CEngineDebugWindow(const CCamera& camera);
    ~CEngineDebugWindow();

    void draw() override;

private:
    const CCamera& mCamera;
};
