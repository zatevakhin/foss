#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>


class ICamera
{

public:
    virtual void update(float delta) = 0;

    virtual glm::mat4 get_view() const = 0;
    virtual glm::mat4 get_projection() const = 0;
    virtual glm::vec3 get_position() const = 0;

    virtual float get_moving_speed() const = 0;
    virtual void set_moving_speed(float speed) = 0;

    virtual void set_near_far(const glm::vec2 near_far) = 0;
    virtual glm::vec2 get_near_far() const = 0;

    virtual void set_fov(const float fov) = 0;
    virtual float get_fov() = 0;
};
