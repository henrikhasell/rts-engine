#include "scene.hpp"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

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
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path,
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate           |
        aiProcess_GenSmoothNormals      |
        aiProcess_FlipWindingOrder      );

    vertexBuffer.clear();
    normalBuffer.clear();
    indexBuffer.clear();

    if(scene)
    {
        for(unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[i];

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
        }
    }

    return scene != nullptr;
}

void Scene::createMesh(Mesh3D &mesh)
{
    mesh.setVertices(vertexBuffer);
    mesh.setNormals(normalBuffer);
    mesh.setIndices(indexBuffer);
}

