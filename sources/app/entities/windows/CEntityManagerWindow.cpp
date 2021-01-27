
#include "app/components/CEditableComponent.hpp"
#include "app/components/CParticleSystemComponent.hpp"
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

    for (auto [entity, c] : m_entity_manager.getEntitySet<CEditableComponent>())
    {
        auto [editable] = c;
        auto name = fmt::format("{} ({})", editable.get_name(), static_cast<size_t>(entity));
        editable_entity_list.emplace_back(name, entity);
    }

    ImGui::Begin("Entity manager");
    ImGui::ListBox("", &current_entity, vector_getter, static_cast<void*>(&editable_entity_list),
                   editable_entity_list.size(), 6);

    auto entity = editable_entity_list[current_entity].second;

    if (m_entity_manager.hasComponent<CTransform3DComponent>(entity))
    {
        auto& component = m_entity_manager.getComponent<CTransform3DComponent>(entity);

        auto title = fmt::format("Transform ({})", static_cast<size_t>(entity));
        if (ImGui::CollapsingHeader(title.c_str()))
        {
            {
                auto& position = component.mPosition;
                ImGui::InputFloat3("Position", &position[0]);
            }

            {
                auto scale = component.mScale;
                ImGui::InputFloat3("Scale", &scale[0]);
            }

            {
                // v - radians
                auto v = glm::eulerAngles(glm::normalize(component.mOrientation));
                ImGui::SliderAngle("Orientation.x", &v.x, -180.f, 180.f);
                ImGui::SliderAngle("Orientation.y", &v.y, -89.f, 89.f);
                ImGui::SliderAngle("Orientation.z", &v.z, -180.f, 180.f);
                component.mOrientation = glm::quat(v);
            }
        }
    }

    if (m_entity_manager.hasComponent<CParticleSystemComponent>(entity))
    {
        auto& component = m_entity_manager.getComponent<CParticleSystemComponent>(entity);
        auto title = fmt::format("Particle system ({})", static_cast<size_t>(entity));
        if (ImGui::CollapsingHeader(title.c_str()))
        {
            {
                auto scale = component.mParticleSystem->get_particle_scale();
                ImGui::InputFloat2("Particle scale", &scale[0]);
                component.mParticleSystem->set_particle_scale(scale);
            }

            {
                auto gravity = component.mParticleSystem->get_gravity();
                ImGui::InputFloat3("Gravity", &gravity[0]);
                component.mParticleSystem->setGravity(gravity);
            }
        }

        auto title2 = fmt::format("Particle emitter ({})", static_cast<size_t>(entity));
        if (ImGui::CollapsingHeader(title2.c_str()))
        {
            {
                ImVec4 text_color(1.f, 1.f, 0.f, 1.f);
                auto count = component.mParticleSystem->get_patricles_count();
                ImGui::TextColored(text_color, "Particles count: %lu", count);
            }

            // {
            //     auto position = component.m_particle_emitter->get_position();
            //     ImGui::InputFloat3("Position", &position[0]);
            //     component.m_particle_emitter->setPosition(position);
            // }

            // {
            //     auto direction = component.m_particle_emitter->get_direction();
            //     ImGui::InputFloat3("Direction", &direction[0]);
            //     component.m_particle_emitter->setDirection(direction);
            // }

            {
                auto range = component.m_particle_emitter->get_distance_range();
                ImGui::InputFloat2("Distance range", &range[0]);
                component.m_particle_emitter->setDistanceRange(range.x, range.y);
            }

            {
                auto range = component.m_particle_emitter->get_emit_interval_range();
                ImGui::SliderFloat2("Emit interval", &range[0], 0.00001f, 2.0f, "%.5f", 3.f);
                component.m_particle_emitter->setEmitIntervalRange(range.x, range.y);
            }

            {
                auto range = component.m_particle_emitter->get_lifetime_range();
                ImGui::SliderFloat2("Lifetime range", &range[0], 0.0001f, 100.0f, "%.4f", 3.f);
                component.m_particle_emitter->setLifetimeRange(range.x, range.y);
            }

            {
                auto range = component.m_particle_emitter->get_speed_range();
                ImGui::SliderFloat2("Speed range", &range[0], -100.f, 100.0f, "%.3f", 3.f);
                component.m_particle_emitter->setSpeedRange(range.x, range.y);
            }

            {
                auto angle = component.m_particle_emitter->get_max_deviation_angle();
                ImGui::SliderAngle("Deviation angle", &angle, -180.f, 180.f);
                component.m_particle_emitter->setMaxDeviationAngle(angle);
            }
        }
    }

    ImGui::End();
}
