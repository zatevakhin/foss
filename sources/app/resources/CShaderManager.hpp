#pragma once

#include "app/shading/IShaderProgram.hpp"
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>


class CShaderManager
{

public:
    CShaderManager(std::string shadersDirectory);
    ~CShaderManager() = default;

    CShaderManager(const CShaderManager&) = delete;
    CShaderManager& operator=(const CShaderManager&) = delete;

    void initialize();

    // void reloadModified(); // check by modification time
    void reloadByName(const char* shaderName);
    TProgramSharedPtr getByName(const char* shaderName) const;
    void use(const char* shaderName) const;

    std::vector<std::pair<std::string, unsigned int>> getProgramsList() const;

private:
    std::string getShaderPath(const char* shaderName) const;
    std::tuple<unsigned int, std::vector<unsigned int>>
    getShaderByPath(const char* shaderName) const;

private:
    std::unordered_map<std::string, TProgramSharedPtr> mPrograms;
    std::string mShadersDirectory;
};


using TShaderManagerPtr = std::shared_ptr<CShaderManager>;
