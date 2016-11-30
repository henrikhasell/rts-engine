#include "animation.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <iostream>

using namespace Engine;
using namespace GL;

AnimatedMesh::AnimatedMesh()
{
    glGenBuffers(6, &buffer[0]);
}

AnimatedMesh::~AnimatedMesh()
{
    glDeleteBuffers(6, &buffer[0]);
}

bool AnimatedMesh::loadScene(const aiScene *scene)
{
    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *currentMesh = scene->mMeshes[i];

        if(!currentMesh->mNumVertices || !currentMesh->mNormals || !currentMesh->mNumFaces)
        {
            std::cerr << "The mesh does not contain necessary data. Skipping!" << std::endl;
            continue;
        }

        std::vector<glm::vec3> position;
        std::vector<glm::vec3> normal;
        std::vector<glm::vec2> textureCoord;
        std::vector<glm::vec4> colour;

        for(unsigned int j = 0; j < currentMesh->mNumVertices; j++)
        {
            const aiVector3D *currentPosition = currentMesh->mVertices + j;
            const aiVector3D *currentNormal = currentMesh->mNormals + j;
            const aiVector3D *currentTextureCoord = currentMesh->mTextureCoords[j];
            const aiColor4D *currentColour = currentMesh->mColors[j];

            const glm::vec3 vp(currentPosition->x, currentPosition->y, currentPosition->z);
            position.push_back(vp);

            const glm::vec3 vn(currentNormal->x, currentNormal->y, currentNormal->z);
            normal.push_back(vn);

            if(currentTextureCoord)
            {
                const glm::vec2 vt(currentTextureCoord->x, 1.0f - currentTextureCoord->y);
                textureCoord.push_back(vt);
            }
            else
            {
                std::cerr << "Warning: Vertex does not supply texture coord information." << std::endl;
                textureCoord.emplace_back(0.0, 0.0);
            }

            if(currentColour)
            {
                const glm::vec4 vc(currentColour->r, currentColour->g, currentColour->b, currentColour->a);
                colour.push_back(vc);
            }
            else
            {
                std::cerr << "Warning: Vertex does not supply colour information." << std::endl;
                colour.emplace_back(1.0, 1.0, 1.0, 1.0);
            }
        }

        std::vector<unsigned int> indices;

        for(unsigned int j = 0; j < currentMesh->mNumFaces; j++)
        {
            aiFace *currentFace = currentMesh->mFaces + j;

            for(unsigned int k = 0; k < currentFace->mNumIndices; k++)
            {
                indices.emplace_back(currentFace->mIndices[k]);
            }
        }

        for(unsigned int j = 0; j < currentMesh->mNumBones; j++)
        {
            aiBone *currentBone = currentMesh->mBones[j];

            if(!currentBone)
            {
                continue;
            }

            if(currentBone->mNumWeights > 4)
            {
                std::cerr << "Number of bone weights exceeds 4!" << std::endl;
            }
            else
            {
                std::cout << "Number of bone weights: " << currentBone->mNumWeights << std::endl;
            }

            for(unsigned int k = 0; k < currentBone->mNumWeights; k++)
            {

            }
        }
    }
    return false;
}

bool AnimatedMesh::loadFile(const char path[])
{
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path,
        aiProcess_Triangulate     |
        aiProcess_GenSmoothNormals|
        aiProcess_GenUVCoords     |
        aiProcess_SortByPType
        );

    loadScene(scene);
}
