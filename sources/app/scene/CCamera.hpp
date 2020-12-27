#pragma once

#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/sdl.hpp"
#include "app/auxiliary/glm.hpp"

#include <set>


enum class ECameraMoveDirection
{
    eForward,
    eBack,
    eLeft,
    eRight,
    eNone
};

class CCamera
{
public:
    CCamera();

    ~CCamera() = default;

    CCamera( const CCamera& ) = delete;
    CCamera& operator=( const CCamera& ) = delete;

    void update(double delta);

    glm::mat4 getView() const;
    glm::mat4 getProjection() const;
    glm::vec3 getPosition() const;

    double getMoveSpeed() const;
    void setMoveSpeed(double speed);

    void setNearAndFar(const glm::dvec2 nf);
    glm::dvec2 getNearAndFar() const;

    void setFov(const double fov);
    double getFov();

    void addMoveDirection(const ECameraMoveDirection& direction);
    void removeMoveDirection(const ECameraMoveDirection& direction);
    void mouseMove(const glm::ivec2& delta);


private:
    bool mIsActive;
    double mSpeed;
    double mFov;

    std::set<ECameraMoveDirection> mDirections;
    glm::vec3 mPosition;
    glm::vec3 mForward;
    glm::vec3 mUp;

    glm::dvec2 mNearAndFar;
};
