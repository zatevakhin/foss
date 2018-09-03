
#ifndef FOSS_APP_ENTITIES_WINDOWS_CENGINESETTINGSWINDOW_HPP
#define FOSS_APP_ENTITIES_WINDOWS_CENGINESETTINGSWINDOW_HPP

#include "IWindow.hpp"


struct SEngineSettings;


class CEngineSettingsWindow : public IWindow
{
public:
    CEngineSettingsWindow(SEngineSettings & settings);
    ~CEngineSettingsWindow();

    void draw() override;

private:
    SEngineSettings & mSettings;
};

#endif //FOSS_APP_ENTITIES_WINDOWS_CENGINESETTINGSWINDOW_HPP
