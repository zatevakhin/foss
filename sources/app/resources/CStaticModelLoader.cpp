
#include "app/textures/ITexture.hpp"
#include "CStaticModelLoader.hpp"
#include "app/geometry/auxiliary.hpp"
#include "auxiliary.hpp"
#include "app/auxiliary/trace.hpp"

#include <assimp/scene.h>
#include <assimp/types.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <unordered_map>

namespace
{
    template<class T>
    size_t getBytesCount(const std::vector<T> & data)
    {
        return sizeof(T) * data.size();
    }

    template<class T>
    T *addItemsToWrite(std::vector<T> & data, size_t count)
    {
        const size_t oldSize = data.size();
        data.resize(oldSize + count);

        return (data.data() + oldSize);
    }

    geometry::EPrimitiveType mapPrimitiveType(unsigned int aiPrimitiveTypeValue)
    {
        switch (aiPrimitiveTypeValue)
        {
            case aiPrimitiveType_POINT:
                return geometry::EPrimitiveType::ePoints;
            case aiPrimitiveType_LINE:
                return geometry::EPrimitiveType::eLines;
            case aiPrimitiveType_TRIANGLE:
                return geometry::EPrimitiveType::eTriangles;
            default:
                break;
        }
        throw std::runtime_error("Unsupported assimp primitive type " + std::to_string(aiPrimitiveTypeValue));
    }

    unsigned int getPrimitiveVertexCount(geometry::EPrimitiveType type)
    {
        switch (type)
        {
            case geometry::EPrimitiveType::ePoints:
                return 1U;
            case geometry::EPrimitiveType::eLines:
                return 2U;
            case geometry::EPrimitiveType::eTriangles:
                return 3U;
            default:
                break;
        }
        throw std::runtime_error("Unexpected internal primitive type " + std::to_string(static_cast<unsigned>(type)));
    }


    class CMeshAccumulator
    {
    public:
        static const size_t RESERVED_SIZE = (4 * 1024);

        CMeshAccumulator()
        {
            mGeometry.mIndexes.reserve(RESERVED_SIZE);
            mGeometry.mVerticies.reserve(RESERVED_SIZE);
        }

        void collectBoundingBox(const aiScene & scene)
        {
            mGeometry.mBBox = getNodeBBox(scene, *scene.mRootNode);
        }

        void visitNodeTree(const aiScene & scene)
        {
            visitNode(*scene.mRootNode, glm::mat4(1));
        }

        void add(const aiMesh & mesh)
        {
            const auto meshNo = unsigned(mMeshes.size());
            CStaticMesh3D mesh3d;
            mesh3d.mMaterialIndex = mesh.mMaterialIndex;

            try
            {
                mesh3d.mLocal = mMeshTransforms.at(meshNo);
            }
            catch (const std::out_of_range &)
            {
                throw std::out_of_range("Submesh #" + std::to_string(meshNo) + " has no transform");
            }

            setupBytesLayout(mesh, mesh3d.mLayout);
            copyVertexes(mesh, mesh3d.mLayout);
            copyIndexes(mesh, mesh3d.mLayout);

            mMeshes.push_back(mesh3d);
        }

        std::vector<CStaticMesh3D> && takeMeshes()
        {
            return std::move(mMeshes);
        }

        geometry::CGeometrySharedPtr makeGeometry() const
        {
            auto geometry = std::make_shared<geometry::CGeometry>();
            geometry->copy(mGeometry);
            return geometry;
        }

    private:

        static geometry::CBoundingBox getNodeBBox(const aiScene & scene, const aiNode & node)
        {
            glm::vec3 lowerBound(0);
            glm::vec3 upperBound(0);

            for (unsigned mi = 0; mi < node.mNumMeshes; ++mi)
            {
                const unsigned indexOnScene = node.mMeshes[mi];
                const aiMesh* mesh = scene.mMeshes[indexOnScene];
                for (unsigned vi = 0; vi < mesh->mNumVertices; ++vi)
                {
                    const aiVector3D aiVertex = mesh->mVertices[vi];
                    const glm::vec3 vertex = glm::vec3(aiVertex.x, aiVertex.y, aiVertex.z);
                    lowerBound = glm::min(lowerBound, vertex);
                    upperBound = glm::max(upperBound, vertex);
                }
            }

            geometry::CBoundingBox box(lowerBound, upperBound);

            for (unsigned ci = 0; ci < node.mNumChildren; ++ci) {
                const aiNode* pChildNode = node.mChildren[ci];
                geometry::CBoundingBox childBox = getNodeBBox(scene, *pChildNode);
                box.unite(childBox);
            }

            return box;
        }

        void setupBytesLayout(const aiMesh & mesh, geometry::SGeometryLayout & layout)
        {
            const geometry::EPrimitiveType primitive = mapPrimitiveType(mesh.mPrimitiveTypes);
            const unsigned vertexPerPrimitive = getPrimitiveVertexCount(primitive);

            layout.mPrimitive = primitive;
            layout.mIndexCount = size_t(vertexPerPrimitive * mesh.mNumFaces);
            layout.mVertexCount = size_t(mesh.mNumVertices);
            layout.mBaseVertexOffset = getBytesCount(mGeometry.mVerticies);
            layout.mBaseIndexOffset = getBytesCount(mGeometry.mIndexes);

            layout.mPosition3D = layout.mVertexSize;
            layout.mVertexSize += sizeof(aiVector3D);

            layout.mNormal = layout.mVertexSize;
            layout.mVertexSize += sizeof(aiVector3D);

            if (mesh.HasTextureCoords(0))
            {
                layout.mTexCoord2D = layout.mVertexSize;
                layout.mVertexSize += sizeof(aiVector2D);
            }

            if (mesh.HasTangentsAndBitangents())
            {
                layout.mTangent = layout.mVertexSize;
                layout.mVertexSize += sizeof(aiVector3D);
                layout.mBitangent = layout.mVertexSize;
                layout.mVertexSize += sizeof(aiVector3D);
            }
        }

        void copyVertexes(const aiMesh& mesh, geometry::SGeometryLayout &layout)
        {
            const size_t dataSize = layout.mVertexCount * layout.mVertexSize;
            uint8_t *dest = addItemsToWrite(mGeometry.mVerticies, dataSize);
            for (unsigned i = 0, n = mesh.mNumVertices; i < n; i += 1)
            {
                std::memcpy(dest + layout.mPosition3D,
                            &mesh.mVertices[i].x, sizeof(aiVector3D));
                std::memcpy(dest + layout.mNormal,
                            &mesh.mNormals[i].x, sizeof(aiVector3D));

                if (layout.mTexCoord2D != geometry::SGeometryLayout::UNSET)
                {
                    std::memcpy(dest + layout.mTexCoord2D,
                                &mesh.mTextureCoords[0][i].x, sizeof(aiVector2D));
                }

                if (layout.mTangent != geometry::SGeometryLayout::UNSET)
                {
                    std::memcpy(dest + layout.mTangent,
                                &mesh.mTangents[i].x, sizeof(aiVector3D));
                }

                if (layout.mBitangent != geometry::SGeometryLayout::UNSET)
                {
                    std::memcpy(dest + layout.mBitangent,
                                &mesh.mBitangents[i].x, sizeof(aiVector3D));
                }

                dest += layout.mVertexSize;
            }
        }

        void copyIndexes(const aiMesh& mesh, geometry::SGeometryLayout &layout)
        {
            const unsigned vertexPerPrimitive = getPrimitiveVertexCount(layout.mPrimitive);
            const size_t dataSize = vertexPerPrimitive * mesh.mNumFaces;
            uint32_t *dest = addItemsToWrite(mGeometry.mIndexes, dataSize);

            for (unsigned i = 0, n = mesh.mNumFaces; i < n; i += 1)
            {
                unsigned *indicies = mesh.mFaces[i].mIndices;
                std::memcpy(dest, indicies, sizeof(unsigned) * vertexPerPrimitive);
                dest += vertexPerPrimitive;
            }
        }

        void visitNode(const aiNode &node, const glm::mat4 &parentTransform)
        {
            const glm::mat4 globalMat4 = parentTransform * glm::transpose(glm::make_mat4(&node.mTransformation.a1));

            for (auto mi = 0U; mi < node.mNumMeshes; ++mi)
            {
                const unsigned int meshNo = node.mMeshes[mi];
                if (mMeshTransforms.count(meshNo))
                {
                    throw std::runtime_error("Mesh #" + std::to_string(meshNo) + " used twice in node tree");
                }
                mMeshTransforms[meshNo] = globalMat4;
            }
            for (auto ci = 0U; ci < node.mNumChildren; ++ci)
            {
                visitNode(*node.mChildren[ci], globalMat4);
            }
        }

        std::vector<CStaticMesh3D> mMeshes;
        geometry::SGeometryData<uint8_t, uint32_t> mGeometry;
        std::unordered_map<unsigned, glm::mat4> mMeshTransforms;
    };
}

CStaticModelLoader::CStaticModelLoader(CResourceLoader &resourceLoader)
    : mResourceLoader(resourceLoader)
{
}

SStaticModel3DPtr CStaticModelLoader::load(const boost::filesystem::path &path)
{
    const boost::filesystem::path absPath = CResourceLoader::getAbsolute(path);

    Assimp::Importer importer;
    const aiScene &scene = openScene(absPath, importer);

    CMeshAccumulator accumulator;
    accumulator.collectBoundingBox(scene);
    accumulator.visitNodeTree(scene);

    for (auto mi = 0U; mi < scene.mNumMeshes; ++mi)
    {
        accumulator.add(*(scene.mMeshes[mi]));
    }

    auto model = std::make_shared<SStaticModel3D>();
    model->mMeshes = accumulator.takeMeshes();
    model->mGeometry = accumulator.makeGeometry();

    resources::loadMaterials(
        absPath.parent_path(),
        mResourceLoader,
        scene,
        model->mMaterials
        );

    return model;
}

const aiScene &CStaticModelLoader::openScene(const boost::filesystem::path &path, Assimp::Importer &importer, SceneImportQuality quality)
{
    auto importFlags = 0U;
    switch (quality)
    {
        case SceneImportQuality::Fast:
            importFlags = aiProcessPreset_TargetRealtime_Fast;
            break;
        case SceneImportQuality::HighQuality:
            importFlags = aiProcessPreset_TargetRealtime_Quality;
            break;
        case SceneImportQuality::MaxQuality:
            importFlags = aiProcessPreset_TargetRealtime_MaxQuality;
            break;
    }

    const aiScene *scene = importer.ReadFile(path.generic_string().c_str(), importFlags);
    if (scene == nullptr)
    {
        throw std::runtime_error(importer.GetErrorString());
    }

    return *scene;
}
