
#include "CCameraListener.hpp"
#include "core/input/CInputEvent.hpp"
#include "core/scene/CCamera.hpp"

namespace
{

inline eCameraMoveDirection getDirecton(const CInputEvent& event)
{
    eCameraMoveDirection direction;
    switch (event.event().key.keysym.sym)
    {
        case SDLK_w: direction = eCameraMoveDirection::Forward; break;
        case SDLK_s: direction = eCameraMoveDirection::Back;    break;
        case SDLK_a: direction = eCameraMoveDirection::Left;    break;
        case SDLK_d: direction = eCameraMoveDirection::Right;   break;
        default:     direction = eCameraMoveDirection::None;    break;
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
    eCameraMoveDirection direction = getDirecton(event);

    if (eCameraMoveDirection::None != direction)
    {
        mCamera->addMoveDirection(direction);
    }
}

void CCameraListener::onKeyUp(const CInputEvent& event)
{
    eCameraMoveDirection direction = getDirecton(event);

    if (eCameraMoveDirection::None != direction)
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
