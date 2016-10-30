#include "scene.hpp"

#include <iostream>

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/vector3.h>
#include <glm/vec4.hpp>

using namespace Engine;
using namespace GL;

Scene::Scene()
{
    //ctor
}

Scene::~Scene()
{
    //dtor
}

bool Scene::load(const char path[])
{
    const aiScene *scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[i];

        std::vector<glm::vec4> vertexBuffer;
        std::vector<GLuint> indexBuffer;

        for(unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            aiFace *face = mesh->mFaces + j;

            if(face->mNumIndices != 3)
            {
                std::cerr << "Face is not triangular." << std::endl;
                continue;
            }

            for(unsigned int k = 0; k < face->mNumIndices; k++)
            {
                indexBuffer.emplace_back(face->mIndices[k]);
            }
        }

        for(unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            aiVector3D *vertex = mesh->mVertices + j;
            glm::vec4 position(vertex->x, vertex->y, vertex->z, 1.0);
            vertexBuffer.push_back(position);
        }

        this->mesh.emplace_back();

        Mesh &newMesh = this->mesh.back();

        newMesh.setVertices(vertexBuffer);
        newMesh.setIndices(indexBuffer);
    }

    aiReleaseImport(scene);

    return scene != nullptr;
}

void Scene::draw(const Graphics &graphics)
{
    for(Mesh &mesh : this->mesh)
    {
        mesh.draw(graphics);
    }
}
