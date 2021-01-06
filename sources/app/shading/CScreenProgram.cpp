
#include "CScreenProgram.hpp"
#include "CShaderProgram.hpp"
#include "app/resources/CResourceLoader.hpp"


CScreenProgram::CScreenProgram()
    : CShaderProgram()
{
    const auto vsh = CResourceLoader::getFileAsString("resources/shaders/screen.vert");
    const auto fsh = CResourceLoader::getFileAsString("resources/shaders/screen.frag");
    compile(vsh, EShaderType::eVertex);
    compile(fsh, EShaderType::eFragment);
    link();
}
