
#ifndef FOSS_CINPUTEVENTMANAGER_HPP
#define FOSS_CINPUTEVENTMANAGER_HPP

#include "IEventListener.hpp"

#include <SDL2/SDL_events.h>
#include <vector>
#include <map>


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
