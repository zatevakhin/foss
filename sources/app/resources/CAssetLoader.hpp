#pragma once


#include "app/scene/IModel.hpp"

#define JSON_NOEXCEPTION

#define TINYGLTF_USE_CPP14
#define TINYGLTF_NOEXCEPTION

#include "tiny_gltf.h"


class CAssetLoader
{

public:
    CAssetLoader(/* args */) = default;
    ~CAssetLoader() = default;


    TModelPtr getModel(const std::string file);

    /* Return all content, cameras, light, models, materials */
    void getScene(const std::string file);

private:
    void getFromFile(tinygltf::Model& model, std::string file);

private:
    tinygltf::TinyGLTF mLoader;
};
