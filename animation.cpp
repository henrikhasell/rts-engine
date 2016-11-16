#include "animation.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include <iostream>
#include <map>

using namespace Engine;
using namespace GL;
/*
static VertexWeight buildVertexWeight(const aiVertexWeight *vertexWeight)
{
}
static Bone buildBone(const aiBone *bone)
{
}
*/

AnimatedMesh::AnimatedMesh()
{
    //ctor
}

AnimatedMesh::~AnimatedMesh()
{
    //dtor
}

bool AnimatedMesh::load(const char path[])
{
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenUVCoords |
        aiProcess_SortByPType |
        aiProcess_GenNormals
        );

    if(scene)
    {
        std::map<std::string, aiNode*> nodeMap;
        std::map<std::string, aiBone*> boneMap;

        for(unsigned int i = 0; i < scene->mNumAnimations; i++)
        {
        }
    }

    return scene != nullptr;
}

void AnimatedMesh::draw(const Graphics &graphics, Uint32 milliseconds)
{

}
