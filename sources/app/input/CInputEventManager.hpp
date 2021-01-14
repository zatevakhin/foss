#pragma once

#include "IEventListener.hpp"

#include <SDL2/SDL_events.h>
#include <map>
#include <vector>


class CInputEventManager
{
public:
    CInputEventManager();
    ~CInputEventManager();

    void addListener(IEventListener* listener);
    void handle();

private:
    std::vector<IEventListener*> mListeners;

    std::map<int, bool> mKeyStateMap;
};
