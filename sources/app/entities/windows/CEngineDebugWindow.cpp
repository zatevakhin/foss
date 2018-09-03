
#include "CEngineDebugWindow.hpp"
#include "core/scene/CCamera.hpp"
#include "core/auxiliary/imgui.hpp"


CEngineDebugWindow::CEngineDebugWindow(const CCamera & camera)
    : mCamera(camera)
{
}

CEngineDebugWindow::~CEngineDebugWindow()
{
}

void CEngineDebugWindow::draw()
{
    ImGui::Text("App %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    const glm::vec3 & p = mCamera.getPosition();
    const glm::ivec3 & r = glm::degrees(glm::eulerAngles(glm::quat_cast(mCamera.getView())));

    ImGui::Text("Camera(x: %.3f, y: %.3f, z: %.3f)", p.x, p.y, p.z);
    ImGui::Text("Rotation(pitch: %d, yaw: %d, roll: %d)", r.x, r.y, r.z);
}