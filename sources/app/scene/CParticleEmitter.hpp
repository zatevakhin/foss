#pragma once

#include "app/auxiliary/glm.hpp"
#include <random>


class CParticle
{
public:
    CParticle(const glm::vec3& position, const glm::vec3& velocity, float lifetime);

    void advance(float deltaSeconds, const glm::vec3& acceleration);

    glm::vec3 getPosition() const;
    float getAlphaValue() const;
    bool isAlive() const;

private:
    glm::vec3 mPosition;
    glm::vec3 mVelocity;
    float mAlphaValue = 0;
    float mLifetime = 0;
    float mLifetimeInitial = 0;
};

// Based on http://stackoverflow.com/questions/28618900
class CClampedNormalDistribution
{
    std::normal_distribution<float> mDistribution;
    float mMin = 0.f;
    float mMax = 0.f;

public:
    float min() const
    {
        return mMin;
    }

    float max() const
    {
        return mMax;
    }

    // m_distribution requires two parameters:
    // `mean`, those. median value and at the same time mathematical expectation
    // `stddev`, those. standard deviation (variance)
    // we infer these parameters from min / max.
    void param(float min, float max)
    {
        using param_type = std::normal_distribution<float>::param_type;
        const float mean = (min + max) / 2.f;
        const float stddev = (max - min) / 6.f;
        mDistribution.param(param_type(mean, stddev));
        mMin = min;
        mMax = max;
    }

    // Normal distribution produces values over the entire float range
    // But we cut the values out of the range [min, max]
    // Statistically, about 0.3% of the values will be thrown out.
    float operator()(std::mt19937& random)
    {
        while (true)
        {
            float number = mDistribution(random);

            if (number >= mMin && number <= mMax)
            {
                return number;
            }
        }
    }
};


class CParticleEmitter
{
public:
    CParticleEmitter();


    void advance(float dt);
    bool isEmitReady() const;
    CParticle emit();

    void setPosition(const glm::vec3& value);
    glm::vec3 get_position() const;

    void setDistanceRange(float minValue, float maxValue);
    glm::vec2 get_distance_range() const;

    void setDirection(const glm::vec3& value);
    glm::vec3 get_direction() const;

    void setMaxDeviationAngle(float value);
    float get_max_deviation_angle() const;

    void setLifetimeRange(float minValue, float maxValue);
    glm::vec2 get_lifetime_range() const;

    void setEmitIntervalRange(float minValue, float maxValue);
    glm::vec2 get_emit_interval_range() const;

    void setSpeedRange(float minValue, float maxValue);
    glm::vec2 get_speed_range() const;

private:
    using linear_random_float = std::uniform_real_distribution<float>;
    using normal_random_float = CClampedNormalDistribution;

    glm::vec3 MakeRandomDirection();

    float mElapsedSeconds = 0;
    float mNextEmitTime = 0;
    glm::vec3 mPosition;
    glm::vec3 mDirection = glm::vec3(0, 1, 0);
    linear_random_float mDistanceRange;
    linear_random_float mDeviationAngleRange;
    linear_random_float mLifetimeRange;
    linear_random_float mEmitIntervalRange;
    linear_random_float mSpeedRange;
    std::mt19937 mRandom;
};
