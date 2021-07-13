#pragma once

#include "IWindow.hpp"
#include "app/resources/CTextureManager.hpp"


class CTextureManagerWindow : public IWindow
{
public:
    CTextureManagerWindow(TTextureManagerPtr shader_manager);
    ~CTextureManagerWindow() override = default;

    void draw() override;

private:
    TTextureManagerPtr mTextureManager;
};
