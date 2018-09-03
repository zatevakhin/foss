
#include "C3DModelProgram.hpp"
#include "core/shading/CShaderProgram.hpp"
#include "core/resources/CResourceLoader.hpp"

C3DModelProgram::C3DModelProgram()
    : CShaderProgram()
{
    const auto vsh = CResourceLoader::getFileAsString("resources/shaders/m3d.vert");
    const auto fsh = CResourceLoader::getFileAsString("resources/shaders/m3d.frag");
    compile(vsh, eShaderType::Vertex);
    compile(fsh, eShaderType::Fragment);
    link();
}

