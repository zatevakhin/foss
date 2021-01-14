#pragma once

#include "IWindow.hpp"


struct SEngineSettings;
class CCamera;


class CEngineSettingsWindow : public IWindow
{
public:
    CEngineSettingsWindow(SEngineSettings& settings, CCamera& camera);
    ~CEngineSettingsWindow();

    void draw() override;

private:
    SEngineSettings& mSettings;
    CCamera& mCamera;
};
