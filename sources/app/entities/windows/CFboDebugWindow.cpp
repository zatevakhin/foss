
#include "CFboDebugWindow.hpp"
#include "app/auxiliary/imgui.hpp"
#include "app/resources/CRegistry.hpp"
#include "app/textures/CTexture2D.hpp"


void CFboDebugWindow::draw()
{
    ImGui::Begin("FBO [RGB]");
    const auto fboRgb = CRegistry::get<CTexture2D*>("dbg.tex.fbo.rgb");

    if (0 != fboRgb)
    {
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image(reinterpret_cast<void*>(fboRgb->id()), {wsize.x, wsize.y}, ImVec2(0, 1),
                     ImVec2(1, 0));
    } else
    {
        ImGui::Text("Texture not set!");
    }
    ImGui::End();

    ImGui::Begin("FBO [DEPTH]");
    const auto fboDepth = CRegistry::get<CTexture2D*>("dbg.tex.fbo.depth");

    if (0 != fboDepth)
    {
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image(reinterpret_cast<void*>(fboDepth->id()), {wsize.x, wsize.y}, ImVec2(0, 1),
                     ImVec2(1, 0));
    } else
    {
        ImGui::Text("Texture not set!");
    }
    ImGui::End();
}
