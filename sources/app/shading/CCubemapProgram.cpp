
#include "CCubemapProgram.hpp"
#include "core/resources/CResourceLoader.hpp"

CCubemapProgram::CCubemapProgram()
    : CShaderProgram()
{
    const auto vsh = CResourceLoader::getFileAsString("resources/shaders/skybox.vert");
    const auto fsh = CResourceLoader::getFileAsString("resources/shaders/skybox.frag");
    compile(vsh, eShaderType::Vertex);
    compile(fsh, eShaderType::Fragment);
    link();
}

