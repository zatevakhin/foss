#pragma once

#include <SDL2/SDL_events.h>
#include <map>


class CInputEvent
{

public:
    CInputEvent(const SDL_Event & event, const std::map<int, bool>& keyMap);

    inline const SDL_Event & event() const
    {
        return mEvent;
    }

    inline bool isKeyPressed(int key) const
    {
        return mKeyMap.find(key) != mKeyMap.end() && mKeyMap.at(key);
    }

private:
    const SDL_Event & mEvent;
    const std::map<int, bool>& mKeyMap;
};
