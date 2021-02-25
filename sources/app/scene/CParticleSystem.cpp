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
    , mParticleScale(1.f)
    , mMaxParticles(10)
{
    mParticlesVao.bind();

    mParticlePositions.bind();
    gl::vertex_attrib_pointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    mParticlePositions.unbind();

    glVertexAttribDivisor(0, 1);

    mSpriteGeometry.copy(SPRITE_VERTECIES);
    gl::vertex_attrib_pointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

    mParticlesVao.unbind();
}

CParticleSystem::~CParticleSystem()
{
}

void CParticleSystem::setEmitter(std::shared_ptr<CParticleEmitter> emitter)
{
    mEmitter = emitter;
}

void CParticleSystem::setGravity(const glm::vec3& gravity)
{
    mGravity = gravity;
}

glm::vec3 CParticleSystem::getGravity() const
{
    return mGravity;
}

void CParticleSystem::setParticleTexture(const TTextureSharedPtr& texture)
{
    mTexture = texture;
}

void CParticleSystem::setParticleScale(const glm::vec2& scale)
{
    mParticleScale = scale;
}

glm::vec2 CParticleSystem::getParticleScale() const
{
    return mParticleScale;
}

size_t CParticleSystem::getPatriclesCount() const
{
    return mParticles.size();
}

void CParticleSystem::setMaxPatricles(unsigned int maxParticles)
{
    mMaxParticles = maxParticles;
}

unsigned int CParticleSystem::getMaxPatricles() const
{
    return mMaxParticles;
}

void CParticleSystem::advance(float dt, const glm::mat4& transform)
{
    mEmitter->advance(dt);

    while (mEmitter->isEmitReady())
    {
        unsigned int particlesCount = static_cast<unsigned int>(mParticles.size());
        if (particlesCount >= mMaxParticles)
        {
            break;
        }

        mParticles.emplace_back(mEmitter->emit());
    }

    for (auto& particle : mParticles)
    {
        particle.advance(dt, mGravity, transform);
    }

    auto newEnd = std::remove_if(mParticles.begin(), mParticles.end(),
                                 [](const auto& particle) { return !particle.isAlive(); });

    mParticles.erase(newEnd, mParticles.end());

    std::stable_sort(mParticles.begin(), mParticles.end(),
                     [](const CParticle& a, const CParticle& b) {
                         return a.getDistanceFromCamera() < b.getDistanceFromCamera();
                     });

    mPositions.resize(mParticles.size());

    std::transform(mParticles.begin(), mParticles.end(), mPositions.begin(),
                   [](const CParticle& particle) {
                       return glm::vec4(particle.getPosition(), particle.getAlphaValue());
                   });

    mParticlePositions.copy(mPositions);
}

void CParticleSystem::draw(TProgramSharedPtr program)
{
    if (!mTexture)
    {
        spdlog::error("No texture set for particle system");
        return;
    }

    mTexture->bind();

    program->uniform("particle_scale") = mParticleScale;

    const GLsizei vertexCount = GLsizei(SPRITE_VERTECIES.size());
    const GLsizei instanceCount = GLsizei(mParticles.size());

    mParticlesVao.bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceCount);
    mParticlesVao.unbind();
}
