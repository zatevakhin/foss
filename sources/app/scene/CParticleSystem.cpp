#include "CParticleSystem.hpp"
#include "app/auxiliary/glm.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/trace.hpp"
#include "app/shading/CVertexAttribute.hpp"
#include <algorithm>


using glm::vec2;
using glm::vec3;
using glm::vec4;


namespace
{

vec2 SPRITE_VERTECIES[] = {
    {-1.f, -1.f}, {+1.f, -1.f}, {-1.f, +1.f}, {-1.f, +1.f}, {+1.f, -1.f}, {+1.f, +1.f},
};

constexpr size_t SPRITE_VERTEX_COUNT = sizeof(SPRITE_VERTECIES) / sizeof(SPRITE_VERTECIES[0]);
} // namespace


CParticleSystem::CParticleSystem()
    : mSpriteGeometry(EBufferType::eArrayBuffer, EBufferUsage::eStaticDraw)
    , mParticlePositions(EBufferType::eArrayBuffer, EBufferUsage::eStreamDraw)
    , mParticlesVao()
{
    mParticlesVao.bind();

    mParticlePositions.bind();
    gl::vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    mParticlePositions.unbind();

    glVertexAttribDivisor(0, 1);
    mSpriteGeometry.copy(SPRITE_VERTECIES, sizeof(SPRITE_VERTECIES));
    gl::vertex_attrib_pointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);

    mParticlesVao.unbind();
}

CParticleSystem::~CParticleSystem()
{
}

void CParticleSystem::setEmitter(std::unique_ptr<CParticleEmitter>&& pEmitter)
{
    mEmitter = std::move(pEmitter);
}

void CParticleSystem::setGravity(const vec3& gravity)
{
    mGravity = gravity;
}

void CParticleSystem::setParticleTexture(const CTextureSharedPtr& pTexture)
{
    mTexture = pTexture;
}

void CParticleSystem::advance(float dt)
{
    mEmitter->advance(dt);

    while (mEmitter->isEmitReady())
    {
        mParticles.emplace_back(mEmitter->emit());
    }

    for (auto& particle : mParticles)
    {
        particle.advance(dt, mGravity);
    }

    auto newEnd = std::remove_if(mParticles.begin(), mParticles.end(),
                                 [](const auto& particle) { return !particle.isAlive(); });

    mParticles.erase(newEnd, mParticles.end());

    m_isDirty = true;
}

void CParticleSystem::draw(const glm::mat4& worldView)
{
    if (!mTexture)
    {
        spdlog::error("No texture set for particle system");
    }

    mTexture->bind();

    if (m_isDirty)
    {
        updateParticlePositions(worldView);
        m_isDirty = false;
    }

    const GLsizei vertexCount = GLsizei(SPRITE_VERTEX_COUNT);
    const GLsizei instanceCount = GLsizei(mParticles.size());

    mParticlesVao.bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceCount);
    mParticlesVao.unbind();
}

void CParticleSystem::updateParticlePositions(const glm::mat4& worldView)
{
    std::vector<vec3> positions(mParticles.size());
    std::transform(mParticles.begin(), mParticles.end(), positions.begin(),
                   [](const CParticle& particle) { return particle.getPosition(); });

    /// Sort particles in order of distance from the camera.
    std::sort(positions.begin(), positions.end(), [&](const vec3& a, vec3& b) {
        const vec3 viewA = vec3(worldView * vec4(a, 1.0));
        const vec3 viewB = vec3(worldView * vec4(b, 1.0));
        return viewA.z < viewB.z;
    });

    mParticlePositions.copy(positions);
}
