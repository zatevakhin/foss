
#include "CCameraListener.hpp"
#include "CInputEvent.hpp"
#include "app/scene/CCamera.hpp"
#include "resources/CRegistry.hpp"
namespace
{

inline ECameraMoveDirection getDirection(const CInputEvent& event)
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
    const auto& motion = event.event().motion;

    glm::ivec2 mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    CRegistry::set("mouse/position", mouse);

    if (event.isKeyPressed(SDL_BUTTON_RIGHT))
    {
        mCamera->mouseMove(glm::ivec2(motion.xrel, motion.yrel));
    }
}

void CCameraListener::onKeyDown(const CInputEvent& event)
{
    ECameraMoveDirection direction = getDirection(event);

    if (ECameraMoveDirection::eNone != direction)
    {
        mCamera->addMoveDirection(direction);
    }
}

void CCameraListener::onKeyUp(const CInputEvent& event)
{
    ECameraMoveDirection direction = getDirection(event);

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
