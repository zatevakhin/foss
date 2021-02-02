#include "CModelManager.hpp"
#include "app/auxiliary/trace.hpp"


void CModelManager::initialize()
{
    spdlog::debug("{}.{}", typeid(*this).name(), __FUNCTION__);
}


TModelPtr CModelManager::getModel(/*  */)
{
    // TODO: implement models cache.
    return TModelPtr(0);
}
