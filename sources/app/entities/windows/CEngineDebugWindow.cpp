
#include "CEngineDebugWindow.hpp"
#include "app/auxiliary/imgui.hpp"
#include "app/resources/CRegistry.hpp"


CEngineDebugWindow::CEngineDebugWindow(const TFreeCameraPtr& camera)
    : mCamera(camera)
{
}

CEngineDebugWindow::~CEngineDebugWindow()
{
}

void CEngineDebugWindow::draw()
{
    ImGui::Text("App %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

    auto mouse = CRegistry::get<glm::ivec2>("mouse/position");

    const glm::vec3& p = mCamera->get_position();
    const glm::ivec3& r = glm::degrees(glm::eulerAngles(glm::quat_cast(mCamera->get_view())));

    ImGui::Text("Camera(x: %.3f, y: %.3f, z: %.3f)", p.x, p.y, p.z);
    ImGui::Text("Rotation(pitch: %d, yaw: %d, roll: %d)", r.x, r.y, r.z);
    ImGui::Text("Mouse(x: %d, y: %d)", mouse.x, mouse.y);


    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->PushClipRectFullScreen();
    draw_list->AddCircle(ImVec2(1920 / 2, 1080 / 2), 5.f, IM_COL32(255, 0, 255, 255), 12);
    draw_list->AddCircle(ImVec2(mouse.x, mouse.y), 5.f, IM_COL32(255, 255, 0, 255), 12);
    draw_list->PopClipRect();

    ImGui::Begin("ENGINE CONTROLS");

    if (ImGui::Button("EXIT"))
    {
        SDL_Event e;
        e.type = SDL_QUIT;

        SDL_PushEvent(&e);
    }

    ImGui::End();
}
