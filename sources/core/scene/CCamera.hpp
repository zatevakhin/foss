
#ifndef FOSS_CCAMERA_HPP
#define FOSS_CCAMERA_HPP

#include "core/auxiliary/opengl.hpp"
#include "core/auxiliary/sdl.hpp"
#include "core/auxiliary/glm.hpp"

#include <boost/noncopyable.hpp>
#include <anax/Component.hpp>
#include <set>


enum class eCameraMoveDirection
{
    Forward,
    Back,
    Left,
    Right,
    None
};

class CCamera : private boost::noncopyable, public anax::Component
{
public:
    CCamera();

    ~CCamera() = default;

    void update(double delta);

    glm::mat4 getView() const;
    glm::mat4 getProjection() const;
    glm::vec3 getPosition() const;

    double getMoveSpeed() const;
    void setMoveSpeed(double speed);
    void addMoveDirection(const eCameraMoveDirection& direction);
    void removeMoveDirection(const eCameraMoveDirection& direction);
    void mouseMove(const glm::ivec2& delta);

private:
    bool mIsActive;
    double mSpeed;

    std::set<eCameraMoveDirection> mDirections;
    glm::vec3 mPosition;
    glm::vec3 mForward;
    glm::vec3 mUp;
};

#endif //FOSS_CCAMERA_HPP
