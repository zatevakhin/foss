
#include "CCamera.hpp"
#include "app/auxiliary/glm.hpp"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace
{

glm::vec3 CAMERA_EYE = glm::vec3(0, 0, 1);
glm::vec3 CAMERA_AT = glm::vec3(0, 0, 0);
glm::vec3 CAMERA_UP = glm::vec3(0, 1, 0);

glm::ivec2 VIEW_PORT = glm::ivec2(1920, 1080);

constexpr double CAMERA_DEFAULT_SPEED = 1.0;
constexpr double CAMERA_FOV = 90.0;

constexpr double CAMERA_NEAR = 0.1;
constexpr double CAMERA_FAR = 1000.0;

constexpr double CAMERA_RADIANS_IN_PIXEL = 1.0 / 3000.0;


glm::vec3 get_moving_direction(const std::set<ECameraMovingDirection>& directions)
{
    glm::vec3 direction(0.f);

    if (directions.count(ECameraMovingDirection::FORWARD))
    {
        direction.z = +1.f;
    }
    else if (directions.count(ECameraMovingDirection::BACK))
    {
        direction.z = -1.f;
    }

    if (directions.count(ECameraMovingDirection::RIGHT))
    {
        direction.x = -1.f;
    }
    else if (directions.count(ECameraMovingDirection::LEFT))
    {
        direction.x = +1.f;
    }

    if (glm::length(direction) > std::numeric_limits<float>::epsilon())
    {
        direction = glm::normalize(direction);
    }

    return direction;
}

} // namespace


CCamera::CCamera()
    : m_forward(glm::normalize(CAMERA_AT - CAMERA_EYE))
    , m_position(CAMERA_AT)
    , m_up(CAMERA_UP)
    , m_speed(CAMERA_DEFAULT_SPEED)
    , m_is_active(true)
    , m_directions()
    , m_fov(CAMERA_FOV)
    , m_near_far(CAMERA_NEAR, CAMERA_FAR)
{
    const glm::vec3 right = glm::normalize(glm::cross(CAMERA_UP, m_forward));
    m_up = glm::normalize(glm::cross(m_forward, right));
}


void CCamera::update(float delta)
{
    const glm::vec3 direction = get_moving_direction(m_directions);
    const glm::vec3 right = glm::normalize(glm::cross(m_up, m_forward));
    const glm::vec3 orientedDir =
        right * direction.x + m_up * direction.y + m_forward * direction.z;

    m_position += float(m_speed) * float(delta) * orientedDir;
}


glm::mat4 CCamera::get_view() const
{
    return glm::lookAt(m_position, m_position + m_forward, m_up);
}

glm::mat4 CCamera::get_projection() const
{
    const float fov = glm::radians(m_fov);
    const float ratio = static_cast<float>(VIEW_PORT.x) / static_cast<float>(VIEW_PORT.y);
    return glm::perspective(fov, ratio, m_near_far.x, m_near_far.y);
}


glm::vec3 CCamera::get_position() const
{
    return m_position;
}


float CCamera::get_moving_speed() const
{
    return m_speed;
}


void CCamera::set_moving_speed(float speed)
{
    m_speed = speed;
}


void CCamera::set_near_far(const glm::vec2 nf)
{
    m_near_far = nf;
}

glm::vec2 CCamera::get_near_far() const
{
    return m_near_far;
}

void CCamera::set_fov(const float fov)
{
    m_fov = fov;
}

float CCamera::get_fov()
{
    return m_fov;
}

void CCamera::add_moving_direction(const ECameraMovingDirection& direction)
{
    m_directions.insert(direction);
}


void CCamera::remove_moving_direction(const ECameraMovingDirection& direction)
{
    m_directions.erase(direction);
}

void CCamera::mouse_move(const glm::ivec2& delta)
{
    if (m_is_active)
    {
        const float delta_yaw = -delta.x * CAMERA_RADIANS_IN_PIXEL;
        const float delta_roll = -delta.y * CAMERA_RADIANS_IN_PIXEL;

        m_forward = glm::normalize(glm::rotate(m_forward, float(delta_yaw), m_up));

        const glm::vec3 right = glm::normalize(glm::cross(m_forward, m_up));
        m_up = glm::normalize(glm::rotate(m_up, float(delta_roll), right));
        m_forward = glm::normalize(glm::cross(m_up, right));
    }
}
