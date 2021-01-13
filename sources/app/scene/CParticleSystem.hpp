#pragma once

#include <memory>
#include "app/auxiliary/glm.hpp"
#include "CParticleEmitter.hpp"
#include "app/textures/CTexture2D.hpp"
#include "app/scene/CVertexBufferObject.hpp"
#include "app/scene/CVertexArrayObject.hpp"


class CParticleSystem
{
public:
    CParticleSystem();
    ~CParticleSystem();

    CParticleSystem( const CParticleSystem& ) = delete;
    CParticleSystem& operator=( const CParticleSystem& ) = delete;

    void setEmitter(std::unique_ptr<CParticleEmitter> && pEmitter);
    void setGravity(const glm::vec3 &gravity);
    void setParticleTexture(const CTextureSharedPtr &pTexture);

    void advance(float dt);

    void draw(const glm::mat4 &worldView);

private:
    void updateParticlePositions(const glm::mat4 &worldView);

    std::unique_ptr<CParticleEmitter> mEmitter;
    std::vector<CParticle> mParticles;
    glm::vec3 mGravity;

    bool m_isDirty = false;
    CTextureSharedPtr mTexture;
    CVertexArrayObject mParticlesVao;
    CVertexBufferObject mSpriteGeometry;
    CVertexBufferObject mParticlePositions;
};
