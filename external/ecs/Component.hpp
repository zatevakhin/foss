#pragma once

#include <memory>
#include "ComponentContainer.hpp"
#include "ComponentType.hpp"

namespace ecs
{

class BaseComponent
{
public:
    static std::size_t getComponentCount()
    {
        return sFactories.size();
    }

    static std::unique_ptr<BaseComponentContainer> createComponentContainer(std::size_t type)
    {
        return sFactories[type]();
    }

protected:
    template<typename T>
    static ComponentType generateComponentType()
    {
        sFactories.push_back([]() -> std::unique_ptr<BaseComponentContainer>
        {
            return std::make_unique<ComponentContainer<T>>();
        });
        return static_cast<ComponentType>(sFactories.size() - 1);
    }

private:
    using ComponentContainerFactory = std::unique_ptr<BaseComponentContainer>(*)();

    static std::vector<ComponentContainerFactory> sFactories;
};

inline std::vector<BaseComponent::ComponentContainerFactory> BaseComponent::sFactories;

template<typename T>
class Component : public BaseComponent
{
public:
    static const ComponentType Type;
};

template<typename T>
const ComponentType Component<T>::Type = BaseComponent::generateComponentType<T>();

template<typename T>
constexpr void checkComponentType()
{
    static_assert(std::is_base_of_v<Component<T>, T>, "A component of type T must inherit Component<T>");
}

template<typename ...Ts>
constexpr void checkComponentTypes()
{
    (checkComponentType<Ts>(), ...);
}

}