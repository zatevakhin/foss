#pragma once

#include "IWindow.hpp"
#include "app/resources/CShaderManager.hpp"


class CShaderManagerWindow : public IWindow
{
public:
    CShaderManagerWindow(CShaderManager& shaderManager);
    ~CShaderManagerWindow() = default;

    void draw() override;

private:
    CShaderManager& mShaderManager;
};
