#pragma once

#include "IWindow.hpp"


class CFboDebugWindow : public IWindow
{
public:
    CFboDebugWindow() = default;
    ~CFboDebugWindow() = default;

    void draw() override;
};
