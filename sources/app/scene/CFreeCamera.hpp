#pragma once

#include "ICamera.hpp"
#include "app/auxiliary/glm.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/sdl.hpp"

#include <set>


enum class ECameraMovingDirection
{
    FORWARD,
    BACK,
    LEFT,
    RIGHT,
    NONE
};

class CFreeCamera : public ICamera
{
public:
    CFreeCamera();

    ~CFreeCamera() = default;

    CFreeCamera(const CFreeCamera&) = delete;
    CFreeCamera& operator=(const CFreeCamera&) = delete;

    void update(float delta) override;

    glm::mat4 get_view() const override;
    glm::mat4 get_projection() const override;
    glm::vec3 get_position() const override;

    float get_moving_speed() const override;
    void set_moving_speed(float speed) override;

    void set_near_far(const glm::vec2 near_far) override;
    glm::vec2 get_near_far() const override;

    void set_fov(const float fov) override;
    float get_fov() override;

    void add_moving_direction(const ECameraMovingDirection& direction);
    void remove_moving_direction(const ECameraMovingDirection& direction);
    void mouse_move(const glm::ivec2& delta);


private:
    bool m_is_active;
    float m_speed;
    float m_fov;

    std::set<ECameraMovingDirection> m_directions;
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_up;

    glm::vec2 m_near_far;
};


using TFreeCameraPtr = std::shared_ptr<CFreeCamera>;
