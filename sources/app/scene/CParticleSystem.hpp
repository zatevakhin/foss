#pragma once

#include "CParticleEmitter.hpp"
#include "app/auxiliary/glm.hpp"
#include "app/scene/CVertexArrayObject.hpp"
#include "app/scene/CVertexBufferObject.hpp"
#include "app/shading/IShaderProgram.hpp"
#include "app/textures/CTexture2D.hpp"
#include <memory>


class CParticleSystem
{
public:
    CParticleSystem();
    ~CParticleSystem();

    CParticleSystem(const CParticleSystem&) = delete;
    CParticleSystem& operator=(const CParticleSystem&) = delete;

    void setEmitter(std::shared_ptr<CParticleEmitter> emitter);

    void setGravity(const glm::vec3& gravity);
    glm::vec3 getGravity() const;

    void setParticleTexture(const TTextureSharedPtr& texture);

    void setParticleScale(const glm::vec2& scale);
    glm::vec2 getParticleScale() const;

    size_t getPatriclesCount() const;

    void advance(float dt, const glm::mat4& transform);

    void draw(TProgramSharedPtr program);

private:
    void updateParticlePositions(const glm::mat4& worldView);

    std::shared_ptr<CParticleEmitter> mEmitter;
    std::vector<CParticle> mParticles;
    glm::vec3 mGravity;
    glm::vec2 mParticleScale;
    std::vector<glm::vec4> mPositions;

    TTextureSharedPtr mTexture;
    CVertexArrayObject mParticlesVao;
    CVertexBufferObject mSpriteGeometry;
    CVertexBufferObject mParticlePositions;
};
