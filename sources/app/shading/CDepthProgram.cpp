
#include "CDepthProgram.hpp"
#include "CShaderProgram.hpp"
#include "app/resources/CResourceLoader.hpp"


CDepthProgram::CDepthProgram()
    : CShaderProgram()
{
    const auto vsh = CResourceLoader::getFileAsString("resources/shaders/depth.vert");
    const auto fsh = CResourceLoader::getFileAsString("resources/shaders/depth.frag");
    compile(vsh, EShaderType::eVertex);
    compile(fsh, EShaderType::eFragment);
    link();
}
