#pragma once

#include "ecs/Component.hpp"
#include <string>


class CEditableComponent : public ecs::Component<CEditableComponent>
{
public:
    CEditableComponent(std::string name)
        : m_name(name)
    {
    }

    ~CEditableComponent() = default;

    std::string get_name() const
    {
        return m_name;
    }

private:
    std::string m_name;
};
