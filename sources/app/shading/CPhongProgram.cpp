
#include "CPhongProgram.hpp"
#include "core/resources/CResourceLoader.hpp"
#include "core/auxiliary/glm.hpp"

CPhongProgram::CPhongProgram()
    : CShaderProgram()
{
    const auto vsh = CResourceLoader::getFileAsString("resources/shaders/phong.vert");
    const auto fsh = CResourceLoader::getFileAsString("resources/shaders/phong.frag");
    compile(vsh, eShaderType::Vertex);
    compile(fsh, eShaderType::Fragment);
    link();
}
