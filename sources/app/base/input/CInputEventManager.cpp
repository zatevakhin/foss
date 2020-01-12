
#include "app/base/auxiliary/imgui.hpp"

#include "CInputEventManager.hpp"
#include "CInputEvent.hpp"


CInputEventManager::CInputEventManager()
    : mListeners()
{
}


CInputEventManager::~CInputEventManager()
{
    for (const auto& item : mListeners)
    {
        delete item;
    }
}

void CInputEventManager::addListener(IEventListener * listener)
{
    mListeners.push_back(listener);
}

void CInputEventManager::handle()
{
    SDL_Event event{};

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSdlGL3_ProcessEvent(&event);
        switch (event.type)
        {
            case SDL_KEYDOWN:
                mKeyStateMap[event.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                mKeyStateMap[event.key.keysym.sym] = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mKeyStateMap[event.button.button] = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mKeyStateMap[event.button.button] = false;
                break;
            default:
                break;
        }

        for (auto & listener : mListeners)
        {
            if (listener->isAcceptable(event.type))
            {
                listener->accept(CInputEvent(event, mKeyStateMap));
            }
        }
    }
}
