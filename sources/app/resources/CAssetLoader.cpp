
#include "app/auxiliary/sdl.hpp"
#include "app/auxiliary/trace.hpp"

#include "CAssetLoader.hpp"

#include "CTextureManager.hpp"
#include "app/textures/CTexture2D.hpp"

#include "app/geometry/CBoundingBox.hpp"

#include "app/resources/Material.hpp"
#include "app/scene/CVertexArrayObject.hpp"
#include "app/scene/CVertexBufferObject.hpp"
#include "app/scene/Mesh.hpp"
#include "app/scene/Model.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

#include "tiny_gltf.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

namespace
{

class ModelBuilder
{
public:
    ModelBuilder()
        : mVao(std::make_shared<CVertexArrayObject>())
    {
    }

    ~ModelBuilder()
    {
        for (const auto& mesh : mMeshes)
        {
            mesh->free();
        }
    }

    void bind()
    {
        mVao->bind();
    }

    void unbind()
    {
        mVao->unbind();
    }

    void addMesh(TMeshPtr mesh)
    {
        mMeshes.push_back(mesh);
    }

    int addMaterial(TMaterialSharedPtr material)
    {
        mMaterials.push_back(material);
        return static_cast<int>(mMaterials.size()) - 1;
    }

    TModelPtr getModel() const
    {
        return std::make_shared<Model>(mVao, mMeshes, mMaterials);
    }

private:
    TMeshesList mMeshes;
    TMaterialsList mMaterials;
    TVaoSharedPtr mVao;
};

} // namespace

namespace
{

GLenum getImageComponents(const tinygltf::Image& image)
{
    static std::map<int, GLenum> components = {
        {0x01, GL_RED},
        {0x02, GL_RG},
        {0x03, GL_RGB},
        {0x04, GL_RGBA},
    };

    return components.at(image.component);
}

GLenum getImageType(const tinygltf::Image& image)
{
    static std::map<int, GLenum> types = {
        {0x08, GL_UNSIGNED_BYTE},
        {0x0F, GL_UNSIGNED_SHORT},
    };

    return types.at(image.bits);
}


TTextureSharedPtr getTextureFromModel(const tinygltf::Model& model, const int index)
{
    if (index == -1)
    {
        return nullptr;
    }

    const auto& texture = model.textures[index];
    const auto& image = model.images[texture.source];
    const auto& sampler = model.samplers[texture.sampler];

    spdlog::debug("Texture index {}, Texture source {}, Image URI {}", index, texture.source,
                  image.uri);

    auto t = std::make_shared<CTexture2D>();
    t->bind();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    gl::TTexParametriList paremetri = {
        {GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR},
        {GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR},
        {GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT},
        {GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT},
    };

    gl::tex_parameteri(paremetri);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, getImageComponents(image),
                 getImageType(image), &image.image.at(0));

    glGenerateMipmap(GL_TEXTURE_2D);
    t->unbind();

    return t;
}


void bindMesh(ModelBuilder& builder, const tinygltf::Model& model, const tinygltf::Mesh& mesh)
{
    const auto numBufferViews = model.bufferViews.size();
    const auto numBuffers = model.buffers.size();

    spdlog::info("Num buffers: {}", numBuffers);
    spdlog::info("Num buffer views: {}", numBufferViews);

    TVboList vbos;
    vbos.resize(numBufferViews);

    for (size_t i = 0; i < numBufferViews; ++i)
    {
        const auto& bufferView = model.bufferViews[i];
        if (bufferView.target == 0)
        {
            spdlog::warn("Unsupported bufferView");
            continue; // Unsupported bufferView.
            // https://github.com/KhronosGroup/glTF/tree/master/specification/2.0
        }

        const auto& buffer = model.buffers[bufferView.buffer];

        vbos[i] = std::make_unique<CVertexBufferObject>(bufferView.target, GL_STATIC_DRAW);
        vbos[i]->copy(&buffer.data.at(0) + bufferView.byteOffset, bufferView.byteLength);
    }

    TBoundingBoxSharedPtr bbox = nullptr;

    for (const auto& accessor : model.accessors)
    {
        auto isCorrectBuffer = ((0 == accessor.bufferView) || (1 == accessor.bufferView));
        auto isCorrectType = TINYGLTF_TYPE_VEC3 == accessor.type;
        auto isBoundingBoxNotSet = !bbox;

        if (isCorrectBuffer && isCorrectType && isBoundingBoxNotSet)
        {
            const auto& bv = model.bufferViews[accessor.bufferView];
            const auto& buffer = model.buffers[bv.buffer];

            auto begin = &(buffer.data.at(0)) + bv.byteOffset;
            auto end = begin + bv.byteLength;

            auto numPoints = bv.byteLength / sizeof(glm::vec3);
            std::vector<glm::vec3> positions;
            positions.resize(numPoints);

            std::memcpy(&(positions[0]), begin, numPoints * sizeof(glm::vec3));

            bbox = std::for_each(positions.begin(), positions.end(), BoundingBoxBuilder()).get();
        }
    }


    for (size_t i = 0; i < mesh.primitives.size(); ++i)
    {
        auto primitive = mesh.primitives[i];
        auto indexAccessor = model.accessors[primitive.indices];

        for (auto& attrib : primitive.attributes)
        {
            auto accessor = model.accessors[attrib.second];
            auto byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
            vbos[accessor.bufferView]->bind();

            int size = 1;
            if (accessor.type != TINYGLTF_TYPE_SCALAR)
            {
                size = accessor.type;
            }

            int vaa = -1;
            if (attrib.first.compare("POSITION") == 0)
            {
                vaa = 0;
            }
            if (attrib.first.compare("NORMAL") == 0)
            {
                vaa = 1;
            }
            if (attrib.first.compare("TEXCOORD_0") == 0)
            {
                vaa = 2;
            }

            if (vaa > -1)
            {
                auto normalized = accessor.normalized ? GL_TRUE : GL_FALSE;
                gl::vertex_attrib_pointer(vaa, size, accessor.componentType, normalized, byteStride,
                                          BUFFER_OFFSET(accessor.byteOffset));
            }
            else
            {
                spdlog::warn("vaa missing: {}", attrib.first);
            }
        }

        const auto& material = model.materials[primitive.material];

        auto mat = std::make_shared<Material>();

        MeshInfo meshInfo;

        meshInfo.materialId = builder.addMaterial(mat);
        meshInfo.mode = primitive.mode;
        meshInfo.count = indexAccessor.count;
        meshInfo.type = indexAccessor.componentType;

        const auto& pbr = material.pbrMetallicRoughness;

        mat->setName(material.name);

        mat->setBaseColorFactor(glm::make_vec4(&(pbr.baseColorFactor[0])));
        mat->setBaseColorTexture(getTextureFromModel(model, pbr.baseColorTexture.index));

        auto tmr = getTextureFromModel(model, pbr.metallicRoughnessTexture.index);

        mat->setMetallicFactor(pbr.metallicFactor);
        mat->setMetallicTexture(tmr);

        mat->setRoughnessFactor(pbr.roughnessFactor);
        mat->setRoughnessTexture(tmr);

        mat->setEmissiveTexture(getTextureFromModel(model, material.emissiveTexture.index));
        mat->setEmissiveFactor(glm::make_vec3(&(material.emissiveFactor[0])));

        auto tn = getTextureFromModel(model, material.normalTexture.index);
        mat->setNormalTexture(tn, material.normalTexture.scale);

        auto to = getTextureFromModel(model, material.occlusionTexture.index);
        mat->setOcclusionTexture(to, material.occlusionTexture.strength);

        auto mesh = std::make_shared<Mesh>(vbos, bbox, meshInfo);

        builder.addMesh(mesh);
    }
}

void bindModelNodes(ModelBuilder& builder, const tinygltf::Model& model, const tinygltf::Node& node)
{
    if ((node.mesh >= 0) && (node.mesh < model.meshes.size()))
    {
        bindMesh(builder, model, model.meshes[node.mesh]);
    }

    for (size_t i = 0; i < node.children.size(); ++i)
    {
        assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
        bindModelNodes(builder, model, model.nodes[node.children[i]]);
    }
}

void getDefaultScene(ModelBuilder& builder, const tinygltf::Model& model)
{
    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i)
    {
        assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
        bindModelNodes(builder, model, model.nodes[scene.nodes[i]]);
    }
}


} // namespace

TModelPtr CAssetLoader::getModel(const std::string file)
{
    tinygltf::Model model;
    getFromFile(model, file);

    ModelBuilder builder;

    builder.bind();
    getDefaultScene(builder, model);
    builder.unbind();

    spdlog::debug("VBO Map destroyed");

    return builder.getModel();
}


void CAssetLoader::getFromFile(tinygltf::Model& model, std::string file)
{
    std::string err;
    std::string warn;

    auto ret = mLoader.LoadASCIIFromFile(&model, &err, &warn, file.c_str());
    // bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary
    // glTF(.glb)

    if (!warn.empty())
    {
        spdlog::warn("{}", warn.c_str());
    }

    if (!err.empty())
    {
        spdlog::error("{}", err.c_str());
    }

    if (!ret)
    {
        spdlog::error("Failed to parse glTF!");
    }
}
