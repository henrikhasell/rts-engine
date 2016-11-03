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

}

Scene::~Scene()
{

}

bool Scene::load(const char path[])
{
    const aiScene *scene = aiImportFile(path,
        aiProcess_JoinIdenticalVertices
        | aiProcess_Triangulate
        | aiProcess_GenSmoothNormals
        | aiProcess_FlipWindingOrder
    );

    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[i];

        std::vector<glm::vec3> vertexBuffer;
        std::vector<glm::vec3> normalBuffer;
        std::vector<GLuint> indexBuffer;

        for(unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            aiFace *face = mesh->mFaces + j;

            for(unsigned int k = 0; k < face->mNumIndices; k++)
            {
                indexBuffer.emplace_back(face->mIndices[k]);
            }
        }

        for(unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            aiVector3D *vertex = mesh->mVertices + j;
            aiVector3D *vertexN = mesh->mNormals + j;

            glm::vec3 position(vertex->x, vertex->y, vertex->z);
            glm::vec3 normal(vertexN->x, vertexN->y, vertexN->z);

            vertexBuffer.push_back(position);
            normalBuffer.push_back(normal);
        }

        this->mesh.emplace_back();

        Mesh3D &newMesh = this->mesh.back();

        newMesh.setVertices(vertexBuffer);
        newMesh.setNormals(normalBuffer);
        newMesh.setIndices(indexBuffer);
    }

    aiReleaseImport(scene);

    return scene != nullptr;
}

void Scene::draw(const Graphics &graphics, const glm::vec3 &position)
{
    for(Mesh3D &mesh : this->mesh)
    {
        mesh.draw(graphics, position);
    }
}

void Scene::draw(const Graphics &graphics)
{
    for(Mesh3D &mesh : this->mesh)
    {
        mesh.draw(graphics);
    }
}
