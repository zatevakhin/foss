
#include "CCamera.hpp"
#include "auxiliary/glm.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace
{

constexpr glm::vec3 CAMERA_EYE = glm::vec3(0, 0, 1);
constexpr glm::vec3 CAMERA_AT  = glm::vec3(0, 0, 0);
constexpr glm::vec3 CAMERA_UP  = glm::vec3(0, 1, 0);

constexpr glm::ivec2 CAMERA_SIZE  = glm::ivec2(1366, 768);

constexpr double CAMERA_DEFAULT_SPEED  = 1.0;
constexpr double CAMERA_FOV    = 90.0;

constexpr double CAMERA_NEAR   = 0.1;
constexpr double CAMERA_FAR    = 1000.0;

constexpr double CAMERA_RADIANS_IN_PIXEL = 1.0 / 200.0;


glm::vec3 getMoveDirection(const std::set<eCameraMoveDirection> & directions)
{
    glm::vec3 direction(0.f);

    if (directions.count(eCameraMoveDirection::Forward))
    {
        direction.z = +1.f;
    }
    else if (directions.count(eCameraMoveDirection::Back))
    {
        direction.z = -1.f;
    }

    if (directions.count(eCameraMoveDirection::Right))
    {
        direction.x = -1.f;
    }
    else if (directions.count(eCameraMoveDirection::Left))
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
    const double fov = glm::radians(CAMERA_FOV);
    const double ratio = double(CAMERA_SIZE.x) / double(CAMERA_SIZE.y);
    return glm::perspective(fov, ratio, CAMERA_NEAR, CAMERA_FAR);
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


void CCamera::addMoveDirection(const eCameraMoveDirection& direction)
{
    mDirections.insert(direction);
}


void CCamera::removeMoveDirection(const eCameraMoveDirection& direction)
{
    mDirections.erase(direction);
}

void CCamera::mouseMove(const glm::ivec2& delta)
{
    if (mIsActive)
    {
        const double deltaYaw  = -delta.x * CAMERA_RADIANS_IN_PIXEL;
        const double deltaRoll = delta.y * CAMERA_RADIANS_IN_PIXEL;

        mForward = glm::normalize(glm::rotate(mForward, float(deltaYaw), mUp));

        const glm::vec3 right = glm::normalize(glm::cross(mUp, mForward));
        mUp = glm::normalize(glm::rotate(mUp, float(deltaRoll), right));
        mForward = -glm::normalize(glm::cross(mUp, right));
    }
}


