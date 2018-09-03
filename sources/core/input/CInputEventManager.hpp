
#ifndef FOSS_CINPUTEVENTMANAGER_HPP
#define FOSS_CINPUTEVENTMANAGER_HPP

#include <SDL2/SDL_events.h>
#include <vector>
#include <map>

#include "core/input/IEventListener.hpp"

class CInputEventManager
{
public:
    CInputEventManager();
    ~CInputEventManager();

    void addListener(IEventListener* listener);
    void handle();

private:
    std::vector<IEventListener *> mListeners;

    std::map<int, bool> mKeyStateMap;
};

#endif //FOSS_CINPUTEVENTMANAGER_HPP
