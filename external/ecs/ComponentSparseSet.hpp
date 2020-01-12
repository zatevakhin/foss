#pragma once

#include "ComponentId.hpp"
#include "SparseSet.hpp"

namespace ecs
{

template<typename T>
using ComponentSparseSet = SparseSet<ComponentId, T>;

}