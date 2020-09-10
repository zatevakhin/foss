
#include "CCamera.hpp"
#include "app/base/auxiliary/glm.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace
{

glm::vec3 CAMERA_EYE = glm::vec3(0, 0, 1);
glm::vec3 CAMERA_AT  = glm::vec3(0, 0, 0);
glm::vec3 CAMERA_UP  = glm::vec3(0, 1, 0);

glm::ivec2 VIEW_PORT  = glm::ivec2(1920, 1080);

constexpr double CAMERA_DEFAULT_SPEED  = 1.0;
constexpr double CAMERA_FOV    = 90.0;

constexpr double CAMERA_NEAR   = 0.1;
constexpr double CAMERA_FAR    = 1000.0;

constexpr double CAMERA_RADIANS_IN_PIXEL = 1.0 / 3000.0;


glm::vec3 getMoveDirection(const std::set<ECameraMoveDirection> & directions)
{
    glm::vec3 direction(0.f);

    if (directions.count(ECameraMoveDirection::eForward))
    {
        direction.z = +1.f;
    }
    else if (directions.count(ECameraMoveDirection::eBack))
    {
        direction.z = -1.f;
    }

    if (directions.count(ECameraMoveDirection::eRight))
    {
        direction.x = -1.f;
    }
    else if (directions.count(ECameraMoveDirection::eLeft))
    {
        direction.x = +1.f;
    }

    if (glm::length(direction) > std::numeric_limits<float>::epsilon())
    {
        direction = glm::normalize(direction);
    }

    return direction;
}

}


CCamera::CCamera()
    : mForward(glm::normalize(CAMERA_AT - CAMERA_EYE))
    , mPosition(CAMERA_AT)
    , mUp(CAMERA_UP)
    , mSpeed(CAMERA_DEFAULT_SPEED)
    , mIsActive(true)
    , mDirections()
    , mFov(CAMERA_FOV)
    , mNearAndFar(CAMERA_NEAR, CAMERA_FAR)
{
    const glm::vec3 right = glm::normalize(glm::cross(CAMERA_UP, mForward));
    mUp = glm::normalize(glm::cross(mForward, right));
}


void CCamera::update(double delta)
{
    const glm::vec3 direction = getMoveDirection(mDirections);
    const glm::vec3 right = glm::normalize(glm::cross(mUp, mForward));
    const glm::vec3 orientedDir = right * direction.x + mUp * direction.y + mForward * direction.z;

    mPosition += float(mSpeed) * float(delta) * orientedDir;
}


glm::mat4 CCamera::getView() const
{
    return glm::lookAt(mPosition, mPosition + mForward, mUp);
}

glm::mat4 CCamera::getProjection() const
{
    const double fov = glm::radians(mFov);
    const double ratio = static_cast<double>(VIEW_PORT.x) / static_cast<double>(VIEW_PORT.y);
    return glm::perspective(fov, ratio, mNearAndFar.x, mNearAndFar.y);
}


glm::vec3 CCamera::getPosition() const
{
    return mPosition;
}


double CCamera::getMoveSpeed() const
{
    return mSpeed;
}


void CCamera::setMoveSpeed(double speed)
{
    mSpeed = speed;
}


void CCamera::setNearAndFar(const glm::dvec2 nf)
{
    mNearAndFar = nf;
}

glm::dvec2 CCamera::getNearAndFar() const
{
    return mNearAndFar;
}

void CCamera::setFov(const double fov)
{
    mFov = fov;
}

double CCamera::getFov()
{
    return mFov;
}

void CCamera::addMoveDirection(const ECameraMoveDirection& direction)
{
    mDirections.insert(direction);
}


void CCamera::removeMoveDirection(const ECameraMoveDirection& direction)
{
    mDirections.erase(direction);
}

void CCamera::mouseMove(const glm::ivec2& delta)
{
    if (mIsActive)
    {
        const double deltaYaw  = -delta.x * CAMERA_RADIANS_IN_PIXEL;
        const double deltaRoll = -delta.y * CAMERA_RADIANS_IN_PIXEL;

        mForward = glm::normalize(glm::rotate(mForward, float(deltaYaw), mUp));

        const glm::vec3 right = glm::normalize(glm::cross(mForward, mUp));
        mUp = glm::normalize(glm::rotate(mUp, float(deltaRoll), right));
        mForward = glm::normalize(glm::cross(mUp, right));
    }
}


