#pragma once

#include "IWindow.hpp"
#include "app/resources/CShaderManager.hpp"


class CShaderManagerWindow : public IWindow
{
public:
    CShaderManagerWindow(TShaderManagerPtr shader_manager);
    ~CShaderManagerWindow() = default;

    void draw() override;

private:
    TShaderManagerPtr m_shader_manager;
};
