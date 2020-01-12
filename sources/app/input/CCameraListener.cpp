
#include "CCameraListener.hpp"
#include "base/input/CInputEvent.hpp"
#include "base/scene/CCamera.hpp"

namespace
{

inline ECameraMoveDirection getDirecton(const CInputEvent& event)
{
    ECameraMoveDirection direction;
    switch (event.event().key.keysym.sym)
    {
        case SDLK_w: direction = ECameraMoveDirection::eForward; break;
        case SDLK_s: direction = ECameraMoveDirection::eBack;    break;
        case SDLK_a: direction = ECameraMoveDirection::eLeft;    break;
        case SDLK_d: direction = ECameraMoveDirection::eRight;   break;
        default:     direction = ECameraMoveDirection::eNone;    break;
    }

    return direction;
}

}

CCameraListener::CCameraListener(CCamera* camera)
    : mCamera(camera)
{
    mAcceptable = {
        SDL_KEYDOWN,
        SDL_KEYUP,
        SDL_MOUSEMOTION,
        SDL_MOUSEBUTTONDOWN,
        SDL_MOUSEBUTTONUP,
    };
}

bool CCameraListener::isAcceptable(int type)
{
    return mAcceptable.count(type) != 0;
}

void CCameraListener::accept(const CInputEvent& event)
{
    switch (event.event().type)
    {
        case SDL_MOUSEMOTION:
            onMouseMotion(event);
            break;
        case SDL_KEYDOWN:
            onKeyDown(event);
            break;
        case SDL_KEYUP:
            onKeyUp(event);
            break;
        case SDL_MOUSEBUTTONDOWN:
            onMouseButtonDown(event);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseButtonUp(event);
            break;
        default:
            break;
    }
}

void CCameraListener::onMouseMotion(const CInputEvent& event)
{
    if (event.isKeyPressed(SDL_BUTTON_RIGHT))
    {
        const auto& motion = event.event().motion;
        mCamera->mouseMove(glm::ivec2(motion.xrel, motion.yrel));
    }
}

void CCameraListener::onKeyDown(const CInputEvent& event)
{
    ECameraMoveDirection direction = getDirecton(event);

    if (ECameraMoveDirection::eNone != direction)
    {
        mCamera->addMoveDirection(direction);
    }
}

void CCameraListener::onKeyUp(const CInputEvent& event)
{
    ECameraMoveDirection direction = getDirecton(event);

    if (ECameraMoveDirection::eNone != direction)
    {
        mCamera->removeMoveDirection(direction);
    }
}

void CCameraListener::onMouseButtonDown(const CInputEvent& event)
{
    if (event.isKeyPressed(SDL_BUTTON_RIGHT))
    {
        SDL_SetRelativeMouseMode(static_cast<SDL_bool>(true));
    }
}

void CCameraListener::onMouseButtonUp(const CInputEvent& event)
{
    if (!event.isKeyPressed(SDL_BUTTON_RIGHT))
    {
        SDL_SetRelativeMouseMode(static_cast<SDL_bool>(false));
    }
}
