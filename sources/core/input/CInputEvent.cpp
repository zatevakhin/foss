//
// Created by zatevaxin on 11.02.18.
//

#include "CInputEvent.hpp"


CInputEvent::CInputEvent(const SDL_Event & event, const std::map<int, bool>& keyMap)
    : mEvent(event), mKeyMap(keyMap)
{
}
