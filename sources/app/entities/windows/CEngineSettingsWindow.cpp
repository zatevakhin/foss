
#include "CEngineSettingsWindow.hpp"
#include "app/SEngineSettings.hpp"
#include "app/auxiliary/imgui.hpp"
#include "app/scene/CCamera.hpp"


CEngineSettingsWindow::CEngineSettingsWindow(SEngineSettings& settings, CCamera& camera)
    : mSettings(settings)
    , mCamera(camera)
{
}

CEngineSettingsWindow::~CEngineSettingsWindow()
{
}

void CEngineSettingsWindow::draw()
{

    ImGui::ShowDemoWindow();

    ImGui::Begin("ENGINE SETTINGS");
    ImGui::Separator();

    ImGui::Checkbox("GL_BLEND", &mSettings.mBlend);
    ImGui::Checkbox("GL_DEPTH_TEST", &mSettings.mDepthTest);
    ImGui::Checkbox("GL_POLYGON_OFFSET_FILL", &mSettings.mPolygonOffsetFill);
    ImGui::Checkbox("GL_CULL_FACE", &mSettings.mCullFace);

    ImGui::Combo("glPolygonMode(..., {mode})", &mSettings.mPolygonMode.mIndex,
                 &mSettings.mPolygonMode.mItemsNames[0], mSettings.mPolygonMode.mItemsNames.size());

    ImGui::Separator();

    float f0 = mCamera.getMoveSpeed();
    ImGui::SliderFloat("Camera Speed", &f0, 0.0f, 10.0f, "Speed = %.3f");
    mCamera.setMoveSpeed(f0);

    float f1 = mCamera.getFov();
    ImGui::SliderFloat("Camera FoV", &f1, 0.0f, 180.0f, "FoV = %.3f");
    mCamera.setFov(f1);

    glm::vec2 nf = mCamera.getNearAndFar();
    ImGui::SliderFloat("Camera NEAR", &nf.x, 0.01f, 100.0f, "Near = %.3f");
    ImGui::SliderFloat("Camera FAR", &nf.y, 100.0f, 10000.0f, "Far = %.3f");
    mCamera.setNearAndFar(nf);

    ImGui::End();
}
