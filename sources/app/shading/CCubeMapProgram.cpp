
#include "CCubeMapProgram.hpp"
#include "base/resources/CResourceLoader.hpp"


CCubeMapProgram::CCubeMapProgram()
    : CShaderProgram()
{
    const auto vsh = CResourceLoader::getFileAsString("resources/shaders/skybox.vert");
    const auto fsh = CResourceLoader::getFileAsString("resources/shaders/skybox.frag");
    compile(vsh, EShaderType::eVertex);
    compile(fsh, EShaderType::eFragment);
    link();
}
