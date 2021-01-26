
#include "CShaderManager.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/trace.hpp"
#include "app/shading/CBasicProgram.hpp"
#include "app/shading/EShaderType.hpp"
#include "resources.hpp"

#include <filesystem>
#include <iostream>
#include <map>
#include <set>
#include <string>


constexpr const char* VERT_SHADER_EXT = ".vert";
constexpr const char* FRAG_SHADER_EXT = ".frag";
constexpr const char* GEOM_SHADER_EXT = ".geom";


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

    CShaderRaii(const CShaderRaii&) = delete;
    CShaderRaii& operator=(const CShaderRaii&) = delete;

private:
    GLuint mId;

private:
    static GLenum mapShaderType(EShaderType type)
    {
        static const std::map<EShaderType, GLenum> typeMap = {
            {EShaderType::eVertex, GL_VERTEX_SHADER},
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

    return log;
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

void compile(const GLuint programId, std::vector<GLuint>& shaderIds, const std::string& source,
             EShaderType type)
{
    const GLchar* shaderSource[] = {source.c_str()};
    const GLint sourceLength[] = {GLint(source.size())};

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

void load(GLuint& programId, std::vector<GLuint>& shaderIds, std::string& shaderPath)
{
    programId = glCreateProgram();

    std::map<const char*, EShaderType> extensionTypeList = {
        {VERT_SHADER_EXT, EShaderType::eVertex},
        {FRAG_SHADER_EXT, EShaderType::eFragment},
        {GEOM_SHADER_EXT, EShaderType::eGeometry}};

    for (const auto& e : extensionTypeList)
    {
        if (std::filesystem::exists(shaderPath + e.first))
        {
            const auto source = resources::get_content_from(shaderPath + e.first);
            compile(programId, shaderIds, source.str(), e.second);
        }
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

    return shadersList;
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

std::tuple<unsigned int, std::vector<unsigned int>>
CShaderManager::getShaderByPath(const char* shaderName) const
{
    auto shaderPath = getShaderPath(shaderName);
    unsigned int programId;
    std::vector<unsigned int> shaderIds;

    ::load(programId, shaderIds, shaderPath);

    return std::tuple(programId, shaderIds);
}

void CShaderManager::initialize()
{
    for (const auto& e : getShaders(mShadersDirectory))
    {
        auto [programId, shaderIds] = getShaderByPath(e.c_str());
        mPrograms.emplace(e, new CBasicProgram(programId, shaderIds));

        spdlog::debug("Shader [{}] loaded at ({}).", e.c_str(), programId);
    }
}

std::vector<std::pair<std::string, unsigned int>> CShaderManager::getProgramsList() const
{
    std::vector<std::pair<std::string, unsigned int>> programs;

    for (const auto& p : mPrograms)
    {
        programs.emplace_back(p.first, p.second->id());
    }

    return programs;
}

void CShaderManager::reloadByName(const char* shaderName)
{
    auto& shaderPtr = mPrograms.at(shaderName);
    auto [programId, shaderIds] = getShaderByPath(shaderName);

    spdlog::debug("Reloading shader [{}] reloaded ({}) -> ({}).", shaderName, shaderPtr->id(),
                  programId);

    shaderPtr->replace(programId, shaderIds);
}

TProgramWeakPtr CShaderManager::getByName(const char* shaderName) const
{
    return mPrograms.at(shaderName);
}

void CShaderManager::use(const char* shaderName) const
{
    mPrograms.at(shaderName)->use();
}
