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

    void set_emitter(std::shared_ptr<CParticleEmitter> emitter);

    void setGravity(const glm::vec3& gravity);
    glm::vec3 get_gravity() const;

    void setParticleTexture(const TTextureSharedPtr& pTexture);

    void set_particle_scale(const glm::vec2& scale);
    glm::vec2 get_particle_scale() const;

    size_t get_patricles_count() const;

    void advance(float dt);

    void draw(TProgramSharedPtr program, const glm::mat4& worldView);

private:
    void updateParticlePositions(const glm::mat4& worldView);

    std::shared_ptr<CParticleEmitter> m_emitter;
    std::vector<CParticle> mParticles;
    glm::vec3 m_gravity;
    glm::vec2 m_particle_scale;

    bool m_isDirty = false;
    TTextureSharedPtr mTexture;
    CVertexArrayObject mParticlesVao;
    CVertexBufferObject mSpriteGeometry;
    CVertexBufferObject mParticlePositions;
};
