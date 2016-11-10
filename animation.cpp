#include "animation.hpp"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

using namespace Engine;
using namespace GL;

AnimatedMesh::AnimatedMesh() : currentAnimation(nullptr)
{
    //ctor
}

AnimatedMesh::~AnimatedMesh()
{
    //dtor
}

static void _recursiveNodePrint(const aiNode *node, int indentation)
{
    for(int i = 0; i < indentation; i++)
        std::cout << " ";

    std::cout << node->mName.C_Str() << std::endl;

    for(unsigned int i = 0; i < node->mNumChildren; i++)
        _recursiveNodePrint(node->mChildren[i], indentation + 4);
}

static void _displayNodeAnimation(const aiNodeAnim *nodeAnim)
{
    std::cout << "        " << nodeAnim->mNodeName.C_Str() << std::endl <<
    "        " << nodeAnim->mNumPositionKeys << std::endl <<
    "        " << nodeAnim->mNumRotationKeys << std::endl <<
    "        " << nodeAnim->mNumScalingKeys << std::endl;
}

static void _displayAnimationInfo(const aiScene *scene)
{
    for(unsigned int i = 0; i < scene->mNumAnimations; i++)
    {
        aiAnimation *animation = scene->mAnimations[i];
        std::cout << animation->mName.C_Str() << ": " << std::endl <<
        "    Ticks per second:" << animation->mTicksPerSecond << std::endl <<
        "    Duration: " << animation->mDuration << std::endl;

        for(unsigned int i = 0; i < animation->mNumChannels; i++)
        {
            _displayNodeAnimation(animation->mChannels[i]);
        }
    }
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
        /*
        if(scene->mMeshes)
        {
            // TODO: Add support for multiple meshes:
            const aiMesh *mesh = *scene->mMeshes;
        }
        */
        // std::hash<aiString, unsigned int> nameDictionary;
///------------------------------------
///TODO: Check online example.
/// Try to import T3D's Soldier.
/// Checkout the animations inside it.
///------------------------------------
        // Debug print the nodes:
        _recursiveNodePrint(scene->mRootNode, 0);
        _displayAnimationInfo(scene);
    }

    return scene != nullptr;
}

void AnimatedMesh::draw(const Graphics &graphics, Uint32 milliseconds)
{

}
