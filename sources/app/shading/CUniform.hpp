#pragma once

#include <glm/fwd.hpp>
#include <vector>


class CUniform
{

public:
    explicit CUniform(int location);

    void operator=(int value);
    void operator=(float value);
    void operator=(const glm::vec2& value);
    void operator=(const glm::ivec2& value);
    void operator=(const glm::vec3& value);
    void operator=(const glm::vec4& value);
    void operator=(const glm::mat3& value);
    void operator=(const glm::mat4& value);

    void operator=(const std::vector<glm::mat4>& value);

    void operator=(bool) = delete;
    void operator=(double value) = delete;
    void operator=(unsigned value) = delete;
    void operator=(const void*) = delete;

private:
    static constexpr int UNDEFINED = -1;

    int mLocation = UNDEFINED;
};
