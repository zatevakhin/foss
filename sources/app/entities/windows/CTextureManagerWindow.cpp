
#include "CTextureManagerWindow.hpp"
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


CTextureManagerWindow::CTextureManagerWindow(TTextureManagerPtr t)
    : mTextureManager(t)
{
}

void CTextureManagerWindow::draw()
{
    static int currentProgrmName = 0;
    const auto& textures = mTextureManager->getTexturesList();

    const auto numTextures = textures.size();
    std::vector<std::string> texturesNames;
    std::vector<std::string> texturesIds;
    texturesIds.reserve(numTextures);
    texturesNames.reserve(numTextures);

    for (const auto& i : textures)
    {
        texturesIds.emplace_back(i.first);
        texturesNames.emplace_back(i.first + "(" + std::to_string(i.second->id()) + ")");
    }

    ImGui::Begin("Textures manager");
    ImGui::ListBox("", &currentProgrmName, vector_getter, static_cast<void*>(&texturesNames),
                   texturesNames.size(), 6);
    if (numTextures > currentProgrmName)
    {
        auto tex = textures.at(texturesIds[currentProgrmName]);
        if (tex)
        {
            ImGui::Image(reinterpret_cast<void*>(tex->id()), {64, 64}, ImVec2(0, 1), ImVec2(1, 0));
        }
    }

    // if (ImGui::Button("Reload"))
    // {
    //     ImGui::Text("Reloading: %s", texturesNames[currentProgrmName].c_str());
    //     mTextureManager->reloadByName(textures[currentProgrmName].first.c_str());
    // }

    ImGui::End();
}
