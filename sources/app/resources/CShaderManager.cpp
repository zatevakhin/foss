
#include "CShaderManager.hpp"
#include "CResourceLoader.hpp"
#include "app/shading/CBasicProgram.hpp"
#include "app/shading/EShaderType.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/trace.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <map>
#include <set>


constexpr const char* VERT_SHADER_EXT = ".vert";
constexpr const char* FRAG_SHADER_EXT = ".frag";


namespace
{

class CShaderRaii
{
public:
    explicit CShaderRaii(EShaderType type)
    {
        mId = glCreateShader(CShaderRaii::mapShaderType(type));
    }

    ~CShaderRaii()
    {
        glDeleteShader(mId);
    }

    inline const GLuint& id() const
    {
        return mId;
    }

    inline GLuint release()
    {
        const GLuint id = mId;
        mId = 0;
        return id;
    }

    CShaderRaii(const CShaderRaii &) = delete;
    CShaderRaii& operator=(const CShaderRaii &) = delete;

private:
    GLuint mId;

private:

    static GLenum mapShaderType(EShaderType type)
    {
        static const std::map<EShaderType, GLenum> typeMap = {
            {EShaderType::eVertex,   GL_VERTEX_SHADER},
            {EShaderType::eFragment, GL_FRAGMENT_SHADER},
            {EShaderType::eGeometry, GL_GEOMETRY_SHADER},
            // other shader types
        };

        return typeMap.at(type);
    }
};


std::string getInfoLog(GLuint shaderId)
{
    GLsizei infoLogLength = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::string log(size_t(infoLogLength), '\0');
    glGetShaderInfoLog(shaderId, infoLogLength, &infoLogLength, &log[0]);

    // Cut log to real length
    if (size_t(infoLogLength) < log.length())
    {
        log.erase(log.begin() + ptrdiff_t(infoLogLength), log.end());
    }

    return std::move(log);
}

void link(const GLuint programId)
{
    glLinkProgram(programId);

    GLint linkStatus = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE)
    {
        const auto log = getInfoLog(programId);
        std::cout << "Program linking failed: " << log << std::endl;
    }
}

void compile(const GLuint programId, std::vector<GLuint>& shaderIds, const std::string& source, EShaderType type)
{
    const GLchar* shaderSource[] = { source.c_str() };
    const GLint sourceLength[] = { GLint(source.size()) };

    CShaderRaii shaderObject(type);
    glShaderSource(shaderObject.id(), 1, shaderSource, sourceLength);
    glCompileShader(shaderObject.id());

    GLint compileStatus = 0;
    glGetShaderiv(shaderObject.id(), GL_COMPILE_STATUS, &compileStatus);

    if (GL_FALSE == compileStatus)
    {
        const auto log = getInfoLog(shaderObject.id());
        std::cout << "Program compiling failed: " << log << std::endl;
    }

    shaderIds.emplace_back(shaderObject.release());
    glAttachShader(programId, shaderIds.back());
}

void load(GLuint& programId, std::vector<GLuint>& shaderIds, std::map<EShaderType, std::string>& shaders)
{
    /* const auto  */
    programId = glCreateProgram();

    // std::vector<GLuint> shaderIds;

    for (const auto& shader : shaders)
    {
        const auto& source = CResourceLoader::getFileAsString(shader.second);
        compile(programId, shaderIds, source, shader.first);
    }

    link(programId);
}

std::set<std::string> getShaders(std::string dir)
{
    std::set<std::string> shadersList;

    for (const auto& e : std::filesystem::directory_iterator(dir))
    {
        const auto& path = std::filesystem::path(e);
        if (e.is_regular_file() && path.has_extension())
        {
            const auto& ext = path.extension();
            if ((ext == FRAG_SHADER_EXT) || (ext == VERT_SHADER_EXT))
            {
                shadersList.emplace(path.stem());
            }
        }
    }

    return std::move(shadersList);
}

} // namespace


CShaderManager::CShaderManager(std::string shadersDirectory)
    : mShadersDirectory(shadersDirectory)
    , mPrograms()
{
}

std::string CShaderManager::getShaderPath(const char* shaderName) const
{
    return std::filesystem::path(mShadersDirectory) / shaderName;
}

void CShaderManager::initialize()
{
    for (const auto& e : getShaders(mShadersDirectory))
    {
        std::string shaderPath = getShaderPath(e.c_str());

        std::map<EShaderType, std::string> shader = {
            {EShaderType::eVertex, shaderPath + VERT_SHADER_EXT},
            {EShaderType::eFragment, shaderPath + FRAG_SHADER_EXT}
        };

        GLuint programId;
        std::vector<GLuint> shaderIds;

        ::load(programId, shaderIds, shader);

        auto program = std::make_shared<CBasicProgram>(programId, shaderIds);

        mPrograms.emplace(e, program);

        trc_debug("Shader [%s] loaded at (%p).", e.c_str(), program.get());
    }
}


void CShaderManager::reloadByName(const char* shaderName)
{
    auto shaderPtr = mPrograms.find(shaderName);

    if (shaderPtr != mPrograms.end())
    {
        std::string shaderPath = getShaderPath(shaderName);

        std::map<EShaderType, std::string> shader = {
            {EShaderType::eVertex, shaderPath + VERT_SHADER_EXT},
            {EShaderType::eFragment, shaderPath + FRAG_SHADER_EXT}
        };

        GLuint programId;
        std::vector<GLuint> shaderIds;

        ::load(programId, shaderIds, shader);

        auto program = new CBasicProgram(programId, shaderIds);

        trc_debug("Reloading shader [%s] reloaded (%p) -> (%p).", shaderName, shaderPtr->second.get(), program);

        shaderPtr->second.reset(program);
    }
}


CProgramWeakPtr CShaderManager::getByName(const char* shaderName) const
{
    if (mPrograms.find(shaderName) != mPrograms.end())
    {
        std::cout << "Shader (" << shaderName << ") not found." << std::endl;
    }

    return mPrograms.at(shaderName);
}


void CShaderManager::use(const char* shaderName) const
{
    mPrograms.at(shaderName)->use();
}