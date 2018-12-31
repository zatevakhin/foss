
#ifndef FOSS_APP_ENTITIES_WINDOWS_CENGINESETTINGSWINDOW_HPP
#define FOSS_APP_ENTITIES_WINDOWS_CENGINESETTINGSWINDOW_HPP

#include "IWindow.hpp"


struct SEngineSettings;
class CCamera;


class CEngineSettingsWindow : public IWindow
{
public:
    CEngineSettingsWindow(SEngineSettings & settings, CCamera & camera);
    ~CEngineSettingsWindow();

    void draw() override;

private:
    SEngineSettings & mSettings;
    CCamera & mCamera;
};

#endif //FOSS_APP_ENTITIES_WINDOWS_CENGINESETTINGSWINDOW_HPP
