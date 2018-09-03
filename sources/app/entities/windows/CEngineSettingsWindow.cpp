
#include "CEngineSettingsWindow.hpp"
#include "app/SEngineSettings.hpp"
#include "core/auxiliary/imgui.hpp"


CEngineSettingsWindow::CEngineSettingsWindow(SEngineSettings & settings)
    : mSettings(settings)
{
}

CEngineSettingsWindow::~CEngineSettingsWindow()
{
}

void CEngineSettingsWindow::draw()
{
    ImGui::Begin("ENGINE SETTINGS");
    ImGui::Separator();

    ImGui::Checkbox("GL_BLEND",                &mSettings.mBlend);
    ImGui::Checkbox("GL_DEPTH_TEST",           &mSettings.mDepthTest);
    ImGui::Checkbox("GL_POLYGON_OFFSET_FILL",  &mSettings.mPolygonOffsetFill);
    ImGui::Checkbox("GL_CULL_FACE",            &mSettings.mCullFace);

    ImGui::Combo("glPolygonMode(..., {mode})", 
        &mSettings.mPolygonMode.mIndex,
        &mSettings.mPolygonMode.mItemsNames[0],
        mSettings.mPolygonMode.mItemsNames.size()
        );

    ImGui::Separator();
    ImGui::End();
}
