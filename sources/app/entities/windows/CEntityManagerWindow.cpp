
#include "app/components/CCameraComponent.hpp"
#include "app/components/CEditableComponent.hpp"
#include "app/components/CLightComponent.hpp"
#include "app/components/CModelComponent.hpp"
#include "app/components/CParticleSystemComponent.hpp"
#include "app/components/CProceduralComponent.hpp"
#include "app/components/CTransform3DComponent.hpp"

#include "app/scene/CAsteroidGenerator.hpp"
#include "app/scene/CPlanetGenerator.hpp"

#include "CEntityManagerWindow.hpp"
#include "app/auxiliary/imgui.hpp"
#include <fmt/core.h>
#include <set>
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

    if (m_entity_manager.hasComponent<CModelComponent>(entity))
    {
        auto& component = m_entity_manager.getComponent<CModelComponent>(entity);
        auto& debug = component.mDebug;

        auto title = fmt::format("Debug draw ({})", static_cast<size_t>(entity));
        if (ImGui::CollapsingHeader(title.c_str()))
        {
            ImGui::Checkbox("Hide model", &debug.mHideModel);
            ImGui::SameLine(110);
            ImGui::Checkbox("Hide box", &debug.mHideBox);
            ImGui::SameLine(200);
            ImGui::Checkbox("Debug draw", &debug.mEnableDebugDraw);
            ImGui::SameLine(300);
            ImGui::Checkbox("Draw normals", &debug.mEnableNormalsDraw);

            static SModelDebug::EDebugDrawMode modes[3] = {SModelDebug::EDebugDrawMode::FILL,
                                                           SModelDebug::EDebugDrawMode::LINE,
                                                           SModelDebug::EDebugDrawMode::POINT};

            static const char* modeNames[3] = {"FILL", "LINE", "POINT"};
            static int modeIndex = 0;

            modeIndex = static_cast<int>(debug.mDebugDrawMode);

            if (debug.mEnableDebugDraw)
            {
                ImGui::Combo("Draw mode", &modeIndex, &modeNames[0], IM_ARRAYSIZE(modeNames));
            }

            debug.mDebugDrawMode = modes[modeIndex];
        }
    }

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
                auto& scale = component.mScale;
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
                ImVec4 text_color(1.f, 1.f, 0.f, 1.f);
                auto count = component.mParticleSystem->getPatriclesCount();
                ImGui::TextColored(text_color, "Particles count: %lu", count);
            }

            {
                auto maxParticles = static_cast<int>(component.mParticleSystem->getMaxPatricles());
                ImGui::InputInt("Particles max", &maxParticles);
                if (maxParticles >= 0)
                {
                    component.mParticleSystem->setMaxPatricles(maxParticles);
                }
            }

            {
                auto scale = component.mParticleSystem->getParticleScale();
                ImGui::InputFloat2("Particle scale", &scale[0]);
                component.mParticleSystem->setParticleScale(scale);
            }

            {
                auto gravity = component.mParticleSystem->getGravity();
                ImGui::InputFloat3("Gravity", &gravity[0]);
                component.mParticleSystem->setGravity(gravity);
            }
        }

        auto title2 = fmt::format("Particle emitter ({})", static_cast<size_t>(entity));
        if (ImGui::CollapsingHeader(title2.c_str()))
        {
            // {
            //     auto position = component.m_particle_emitter->getPosition();
            //     ImGui::InputFloat3("Position", &position[0]);
            //     component.m_particle_emitter->setPosition(position);
            // }

            // {
            //     auto direction = component.m_particle_emitter->getDirection();
            //     ImGui::InputFloat3("Direction", &direction[0]);
            //     component.m_particle_emitter->setDirection(direction);
            // }

            {
                auto range = component.m_particle_emitter->getDistanceRange();
                ImGui::InputFloat2("Distance range", &range[0]);
                component.m_particle_emitter->setDistanceRange(range.x, range.y);
            }

            {
                auto range = component.m_particle_emitter->getEmitIntervalRange();
                ImGui::SliderFloat2("Emit interval", &range[0], 0.00001f, 2.0f, "%.5f", 3.f);
                component.m_particle_emitter->setEmitIntervalRange(range.x, range.y);
            }

            {
                auto range = component.m_particle_emitter->getLifetimeRange();
                ImGui::SliderFloat2("Lifetime range", &range[0], 0.0001f, 100.0f, "%.4f", 3.f);
                component.m_particle_emitter->setLifetimeRange(range.x, range.y);
            }

            {
                auto range = component.m_particle_emitter->getSpeedRange();
                ImGui::SliderFloat2("Speed range", &range[0], -100.f, 100.0f, "%.3f", 3.f);
                component.m_particle_emitter->setSpeedRange(range.x, range.y);
            }

            {
                auto angle = component.m_particle_emitter->getMaxDeviationAngle();
                ImGui::SliderAngle("Deviation angle", &angle, -180.f, 180.f);
                component.m_particle_emitter->setMaxDeviationAngle(angle);
            }
        }
    }

    if (m_entity_manager.hasComponent<CCameraComponent>(entity))
    {
        auto title = fmt::format("Camera ({})", static_cast<size_t>(entity));
        if (ImGui::CollapsingHeader(title.c_str()))
        {
            auto& component = m_entity_manager.getComponent<CCameraComponent>(entity);
            auto camera = component.getCamera();

            {
                auto speed = camera->get_moving_speed();
                ImGui::SliderFloat("Camera speed", &speed, 0.0f, 100.0f, "Speed = %.3f");
                camera->set_moving_speed(speed);
            }

            {
                auto fov = camera->get_fov();
                ImGui::SliderFloat("Camera FoV", &fov, 0.0f, 179.9f, "FoV = %.3f");
                camera->set_fov(fov);
            }

            {
                auto nearAndFar = camera->get_near_far();
                ImGui::SliderFloat2("Near / Far", &nearAndFar[0], 0.0001f, 10000.0f, "%.5f", 3.f);
                camera->set_near_far(nearAndFar);
            }
        }
    }

    if (m_entity_manager.hasComponent<CProceduralComponent>(entity))
    {
        auto title = fmt::format("CProceduralComponent ({})", static_cast<size_t>(entity));
        if (ImGui::CollapsingHeader(title.c_str()))
        {
            auto& component = m_entity_manager.getComponent<CProceduralComponent>(entity);

            if (EProceduralType::ASTEROID == component.getType())
            {
                const auto& settings = static_cast<TAsteroidSettings&>(
                    const_cast<IProceduralSettings&>(component.getSettings()));

                TAsteroidSettings newSettings{settings};

                ImGui::SliderInt("Object Resolution", &newSettings.mResolution, 10, 1024);

                ImGui::SliderInt("Seed", &newSettings.mSeed, 0, 10);
                ImGui::SliderInt("Raduis", &newSettings.mRadius, 1, 256);

                bool updated = false;
                updated |= settings.mResolution != newSettings.mResolution;
                updated |= settings.mSeed != newSettings.mSeed;
                updated |= settings.mRadius != newSettings.mRadius;

                auto title = fmt::format("Noise Settings ({})", static_cast<size_t>(entity));
                if (ImGui::CollapsingHeader(title.c_str()))
                {
                    auto& noise = newSettings.mNoiseSettings;
                    ImGui::SliderFloat("Strength", &noise.strength, 0.0f, 1.0f, "%.4f");
                    ImGui::SliderFloat("Roughness", &noise.roughness, 0.0f, 5.0f, "%.4f");
                    ImGui::SliderFloat3("Noise center", &noise.center[0], 0.0f, 5.0f, "%.4f", 1.f);

                    updated |= settings.mNoiseSettings.center != noise.center;
                    updated |= settings.mNoiseSettings.roughness != noise.roughness;
                    updated |= settings.mNoiseSettings.strength != noise.strength;
                }

                if (updated)
                {
                    component.setSettings(newSettings);
                }
            }
            else if (EProceduralType::PLANET == component.getType())
            {
                const auto& settings = static_cast<TPlanetSettings&>(
                    const_cast<IProceduralSettings&>(component.getSettings()));

                TPlanetSettings newSettings{settings};

                ImGui::SliderInt("Object Resolution", &newSettings.mResolution, 10, 1024);

                ImGui::SliderInt("Seed", &newSettings.mSeed, 0, 10);
                ImGui::SliderInt("Raduis", &newSettings.mRadius, 1, 256);

                bool updated = false;
                updated |= settings.mResolution != newSettings.mResolution;
                updated |= settings.mSeed != newSettings.mSeed;
                updated |= settings.mRadius != newSettings.mRadius;

                auto title = fmt::format("Noise Settings ({})", static_cast<size_t>(entity));
                if (ImGui::CollapsingHeader(title.c_str()))
                {
                    auto& noise = newSettings.mNoiseSettings;
                    ImGui::SliderFloat("Strength", &noise.strength, 0.0f, 1.0f, "%.4f");
                    ImGui::SliderFloat("Roughness", &noise.roughness, 0.0f, 5.0f, "%.4f");
                    ImGui::SliderFloat3("Noise center", &noise.center[0], 0.0f, 5.0f, "%.4f", 1.f);

                    updated |= settings.mNoiseSettings.center != noise.center;
                    updated |= settings.mNoiseSettings.roughness != noise.roughness;
                    updated |= settings.mNoiseSettings.strength != noise.strength;
                }

                if (updated)
                {
                    component.setSettings(newSettings);
                }
            }
        }
    }

    if (m_entity_manager.hasComponent<CModelComponent>(entity))
    {
        auto title = fmt::format("CModelComponent ({})", static_cast<size_t>(entity));
        if (ImGui::CollapsingHeader(title.c_str()))
        {
            auto& component = m_entity_manager.getComponent<CModelComponent>(entity);
            auto& model = component.mModel;
            auto& meshes = component.mModel->getMaterials();

            std::vector<TPbrMaterialPtr> pbrs;

            for (const auto& mat : meshes)
            {
                pbrs.emplace_back(std::static_pointer_cast<CPbrMaterial>(mat));
            }

            auto isNullptr = [](const auto& a) { return a == nullptr; };

            pbrs.erase(std::remove_if(pbrs.begin(), pbrs.end(), isNullptr), pbrs.end());

            // @note: some generated objects can contain one material for all meshes.
            pbrs.erase(std::unique(pbrs.begin(), pbrs.end()), pbrs.end());

            for (auto& material : pbrs)
            {
                auto title = fmt::format("PBR material ({:#08X})",
                                         reinterpret_cast<std::uintptr_t>(material.get()));

                if (ImGui::CollapsingHeader(title.c_str()))
                {
                    ImGui::SliderFloat("AO", &material->ao, 0.0f, 1.0f, "%.4f");

                    ImGui::SliderFloat("Metallic", &material->metallic, 0.0f, 1.0f, "%.4f");
                    ImGui::SliderFloat("PBR Roughness", &material->roughness, 0.0f, 1.0f, "%.4f");
                    ImGui::ColorEdit3("Albedo", &material->albedo[0], ImGuiColorEditFlags_Float);
                }
            }
        }
    }

    if (m_entity_manager.hasComponent<CLightComponent>(entity))
    {
        auto title = fmt::format("CLightComponent ({})", static_cast<size_t>(entity));
        if (ImGui::CollapsingHeader(title.c_str()))
        {
            auto& component = m_entity_manager.getComponent<CLightComponent>(entity);
            auto light = component.getLight();

            ImGui::InputFloat3("Position", &light->mPosition[0]);
            ImGui::ColorEdit3("Color", &light->mColor[0], ImGuiColorEditFlags_Float);
            ImGui::SliderFloat("Strength", &light->mStrength, 1.0f, 65536.0f, "%.4f", 3.f);
        }
    }

    ImGui::End();
}
