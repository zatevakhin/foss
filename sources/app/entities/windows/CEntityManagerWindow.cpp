
#include "app/components/CEditableComponent.hpp"
#include "app/components/CTransform3DComponent.hpp"

#include "CEntityManagerWindow.hpp"
#include "app/auxiliary/imgui.hpp"
#include <fmt/core.h>
#include <string>
#include <utility>

namespace
{

using TNamedEntityList = std::vector<std::pair<std::string, ecs::Entity>>;

static auto vector_getter = [](void* vec, int idx, const char** out_text) {
    auto& vector = *static_cast<TNamedEntityList*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size()))
    {
        return false;
    }

    *out_text = vector.at(idx).first.c_str();
    return true;
};

} // namespace


CEntityManagerWindow::CEntityManagerWindow(ecs::EntityManager& em)
    : m_entity_manager(em)
{
}

void CEntityManagerWindow::draw()
{
    static int current_entity = 1;
    TNamedEntityList editable_entity_list;

    for (auto [e, c] : m_entity_manager.getEntitySet<CEditableComponent>())
    {
        auto [editable] = c;
        auto entity = static_cast<size_t>(e);

        auto name = editable.get_name() + " (" + std::to_string(entity) + ")";
        editable_entity_list.emplace_back(name, e);
    }

    ImGui::Begin("Entity manager");
    ImGui::ListBox("", &current_entity, vector_getter, static_cast<void*>(&editable_entity_list),
                   editable_entity_list.size(), 6);

    auto entity = editable_entity_list[current_entity].second;

    if (m_entity_manager.hasComponent<CTransform3DComponent>(entity))
    {
        auto& component = m_entity_manager.getComponent<CTransform3DComponent>(entity);

        auto transform = fmt::format("Transform ({})", static_cast<size_t>(entity));
        if (ImGui::CollapsingHeader(transform.c_str()))
        {
            auto orientation =
                glm::degrees(glm::eulerAngles(glm::normalize(component.mOrientation)));

            ImGui::InputFloat("Position.x", &component.mPosition.x, 0.01f, 1.0f);
            ImGui::InputFloat("Position.y", &component.mPosition.y, 0.01f, 1.0f);
            ImGui::InputFloat("Position.z", &component.mPosition.z, 0.01f, 1.0f);
            ImGui::Separator();
            ImGui::InputFloat("Scale.x", &component.mScale.x, 0.01f, 1.0f);
            ImGui::InputFloat("Scale.y", &component.mScale.y, 0.01f, 1.0f);
            ImGui::InputFloat("Scale.z", &component.mScale.z, 0.01f, 1.0f);
            ImGui::Separator();
            ImGui::InputFloat("Orientation.x", &orientation.x, 0.01f, 1.0f);
            ImGui::InputFloat("Orientation.y", &orientation.y, 0.01f, 1.0f);
            ImGui::InputFloat("Orientation.z", &orientation.z, 0.01f, 1.0f);

            component.mOrientation = glm::quat(glm::radians(orientation));
        }
    }

    ImGui::End();
}
