
#include "CEngineListener.hpp"
#include "CEngine.hpp"
#include "CInputEvent.hpp"


CEngineListener::CEngineListener(CEngine& engine)
    : mEngine(engine)
{
    mAcceptable = {
        SDL_QUIT,
    };
}

bool CEngineListener::isAcceptable(int type)
{
    return mAcceptable.count(type) != 0;
}

void CEngineListener::accept(const CInputEvent& event)
{
    switch (event.event().type)
    {
    case SDL_QUIT:
        onQuit(event);
        break;
    default:
        break;
    }
}

void CEngineListener::onQuit(const CInputEvent& event)
{
    mEngine.stop();
}
