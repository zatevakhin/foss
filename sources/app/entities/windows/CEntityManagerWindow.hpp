#pragma once

#include "IWindow.hpp"
#include "ecs/EntityManager.hpp"


class CEntityManagerWindow : public IWindow
{
public:
    CEntityManagerWindow(ecs::EntityManager& em);
    ~CEntityManagerWindow() = default;

    void draw() override;

private:
    ecs::EntityManager& m_entity_manager;
};
