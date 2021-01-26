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

    void setEmitter(std::unique_ptr<CParticleEmitter>&& pEmitter);
    void setGravity(const glm::vec3& gravity);
    void setParticleTexture(const TTextureSharedPtr& pTexture);

    void advance(float dt);

    void draw(TProgramSharedPtr program, const glm::mat4& worldView);

private:
    void updateParticlePositions(const glm::mat4& worldView);

    std::unique_ptr<CParticleEmitter> mEmitter;
    std::vector<CParticle> mParticles;
    glm::vec3 mGravity;

    bool m_isDirty = false;
    TTextureSharedPtr mTexture;
    CVertexArrayObject mParticlesVao;
    CVertexBufferObject mSpriteGeometry;
    CVertexBufferObject mParticlePositions;
};
