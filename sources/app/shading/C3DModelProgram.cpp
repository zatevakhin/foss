
#include "C3DModelProgram.hpp"
#include "base/shading/CShaderProgram.hpp"
#include "base/resources/CResourceLoader.hpp"


C3DModelProgram::C3DModelProgram()
    : CShaderProgram()
{
    const auto vsh = CResourceLoader::getFileAsString("resources/shaders/m3d.vert");
    const auto fsh = CResourceLoader::getFileAsString("resources/shaders/m3d.frag");
    compile(vsh, EShaderType::eVertex);
    compile(fsh, EShaderType::eFragment);
    link();
}
