
#include "CShaderManagerWindow.hpp"
#include "app/auxiliary/imgui.hpp"
#include "app/resources/CRegistry.hpp"
#include "app/textures/CTexture2D.hpp"

namespace
{

static auto vector_getter = [](void* vec, int idx, const char** out_text) {
    auto& vector = *static_cast<std::vector<std::string>*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size()))
    {
        return false;
    }

    *out_text = vector.at(idx).c_str();
    return true;
};

} // namespace


CShaderManagerWindow::CShaderManagerWindow(TShaderManagerPtr shader_manager)
    : m_shader_manager(shader_manager)
{
}

void CShaderManagerWindow::draw()
{
    static int currentProgrmName = 0;
    const auto programs = m_shader_manager->getProgramsList();

    std::vector<std::string> programsNames;
    programsNames.reserve(programs.size());

    for (const auto& i : programs)
    {
        programsNames.emplace_back(i.first + "(" + std::to_string(i.second) + ")");
    }

    ImGui::Begin("Shader manager");
    ImGui::ListBox("", &currentProgrmName, vector_getter, static_cast<void*>(&programsNames), programsNames.size(), 6);

    if (ImGui::Button("Reload"))
    {
        ImGui::Text("Reloading: %s", programsNames[currentProgrmName].c_str());
        m_shader_manager->reloadByName(programs[currentProgrmName].first.c_str());
    }

    ImGui::End();
}
