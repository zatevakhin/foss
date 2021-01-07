#pragma once

#include "app/shading/IShaderProgram.hpp"
#include <memory>
#include <string>
#include <map>


class CShaderManager
{

public:
    CShaderManager(std::string shadersDirectory);
    ~CShaderManager() = default;

    CShaderManager( const CShaderManager& ) = delete;
    CShaderManager& operator=( const CShaderManager& ) = delete;

    void initialize();

    // void reloadModified(); // check by modification time
    void reloadByName(const char* shaderName);
    CProgramWeakPtr getByName(const char* shaderName) const;
    void use(const char* shaderName) const;

private:
    std::string getShaderPath(const char* shaderName) const;
    CProgramSharedPtr getShaderByPath(const char* shaderName) const;

private:
    std::map<std::string, CProgramSharedPtr> mPrograms;
    std::string mShadersDirectory;
};
