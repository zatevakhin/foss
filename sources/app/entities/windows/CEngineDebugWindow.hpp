#ifndef FOSS_APP_ENTITIES_WINDOWS_CENGINEDEBUGWINDOW_HPP
#define FOSS_APP_ENTITIES_WINDOWS_CENGINEDEBUGWINDOW_HPP

#include "IWindow.hpp"


class CCamera;

class CEngineDebugWindow : public IWindow
{
public:
    CEngineDebugWindow(const CCamera & camera);
    ~CEngineDebugWindow();

    void draw() override;
private:
    const CCamera & mCamera;
};

#endif //FOSS_APP_ENTITIES_WINDOWS_CENGINEDEBUGWINDOW_HPP