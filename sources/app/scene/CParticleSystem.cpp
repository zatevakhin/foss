#include "CParticleSystem.hpp"
#include "app/auxiliary/glm.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/trace.hpp"
#include "app/shading/CUniform.hpp"
#include "app/shading/CVertexAttribute.hpp"

#include <algorithm>
#include <array>


namespace
{

constexpr std::array SPRITE_VERTECIES = {
    glm::vec2(-1.f, -1.f), glm::vec2(+1.f, -1.f), glm::vec2(-1.f, +1.f),
    glm::vec2(-1.f, +1.f), glm::vec2(+1.f, -1.f), glm::vec2(+1.f, +1.f),
};

} // namespace


CParticleSystem::CParticleSystem()
    : mSpriteGeometry(EBufferType::eArrayBuffer, EBufferUsage::eStaticDraw)
    , mParticlePositions(EBufferType::eArrayBuffer, EBufferUsage::eStreamDraw)
    , mParticlesVao()
    , m_particle_scale(1.f)
{
    mParticlesVao.bind();

    mParticlePositions.bind();
    gl::vertex_attrib_pointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    mParticlePositions.unbind();

    glVertexAttribDivisor(0, 1);

    mSpriteGeometry.copy(SPRITE_VERTECIES.data(), SPRITE_VERTECIES.size() * sizeof(glm::vec2));
    gl::vertex_attrib_pointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

    mParticlesVao.unbind();
}

CParticleSystem::~CParticleSystem()
{
}

void CParticleSystem::set_emitter(std::shared_ptr<CParticleEmitter> emitter)
{
    m_emitter = emitter;
}

void CParticleSystem::setGravity(const glm::vec3& gravity)
{
    m_gravity = gravity;
}

glm::vec3 CParticleSystem::get_gravity() const
{
    return m_gravity;
}

void CParticleSystem::setParticleTexture(const TTextureSharedPtr& pTexture)
{
    mTexture = pTexture;
}

void CParticleSystem::set_particle_scale(const glm::vec2& scale)
{
    m_particle_scale = scale;
}

glm::vec2 CParticleSystem::get_particle_scale() const
{
    return m_particle_scale;
}

size_t CParticleSystem::get_patricles_count() const
{
    return mParticles.size();
}

void CParticleSystem::advance(float dt)
{
    m_emitter->advance(dt);

    while (m_emitter->isEmitReady())
    {
        mParticles.emplace_back(m_emitter->emit());
    }

    for (auto& particle : mParticles)
    {
        particle.advance(dt, m_gravity);
    }

    auto newEnd = std::remove_if(mParticles.begin(), mParticles.end(),
                                 [](const auto& particle) { return !particle.isAlive(); });

    mParticles.erase(newEnd, mParticles.end());

    m_isDirty = true;
}

void CParticleSystem::draw(TProgramSharedPtr program, const glm::mat4& worldView)
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

    program->uniform("particle_scale") = m_particle_scale;

    const GLsizei vertexCount = GLsizei(SPRITE_VERTECIES.size());
    const GLsizei instanceCount = GLsizei(mParticles.size());

    mParticlesVao.bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceCount);
    mParticlesVao.unbind();
}

void CParticleSystem::updateParticlePositions(const glm::mat4& worldView)
{
    std::vector<glm::vec4> positions(mParticles.size());

    std::transform(mParticles.begin(), mParticles.end(), positions.begin(),
                   [](const CParticle& particle) {
                       return glm::vec4(particle.getPosition(), particle.getAlphaValue());
                   });

    /// Sort particles in order of distance from the camera.
    std::sort(positions.begin(), positions.end(), [&](const glm::vec4& a, const glm::vec4& b) {
        const glm::vec3 viewA = glm::vec3(worldView * glm::vec4(a.x, a.y, a.z, 1.0));
        const glm::vec3 viewB = glm::vec3(worldView * glm::vec4(b.x, b.y, b.z, 1.0));
        return viewA.z < viewB.z;
    });

    mParticlePositions.copy(positions);
}
