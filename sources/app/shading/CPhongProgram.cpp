
#include "CPhongProgram.hpp"
#include "app/resources/CResourceLoader.hpp"
#include "app/auxiliary/glm.hpp"


CPhongProgram::CPhongProgram()
    : CShaderProgram()
{
    const auto vsh = CResourceLoader::getFileAsString("resources/shaders/phong.vert");
    const auto fsh = CResourceLoader::getFileAsString("resources/shaders/phong.frag");
    compile(vsh, EShaderType::eVertex);
    compile(fsh, EShaderType::eFragment);
    link();
}
