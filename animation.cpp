#include "animation.hpp"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <glm/detail/func_matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iterator>
#include <map>

using namespace Engine;
using namespace GL;

#define POSITION_BUFFER 0
#define NORMAL_BUFFER 1
#define TEXCOORD_BUFFER 2
#define BONE_INDEX_BUFFER 3
#define BONE_WEIGHT_BUFFER 4
#define INDEX_BUFFER 5

AnimatedModel::AnimatedModel() : animationRange(nullptr), scene(nullptr)
{

}

AnimatedModel::~AnimatedModel()
{
    if(scene)
    {
        aiReleaseImport(scene);
    }
}

    bool AnimatedModel::setAnimationRange(const char name[])
    {
        const std::string animationName(name);

        std::map<std::string, std::pair<double, double>>::iterator i = animationRanges.find(animationName);
        bool result = i != animationRanges.end();

        if(result)
        {
            animationRange = &(*i).second;
        }
        else
        {
            std::cerr << "Cannot find animation " << animationName << std::endl;
        }

        return result;
    }
    void AnimatedModel::addAnimationRange(const char name[], double start, double finish)
    {
        animationRanges[std::string(name)] = std::pair<double, double>(start, finish);
    }

void AnimatedModel::setTexture(size_t index, const char path[])
{
    textureArray[index].load(path);
}

bool AnimatedModel::loadFile(const char path[])
{
    if(scene)
    {
        aiReleaseImport(scene);
    }

    scene = aiImportFile(path,
        aiProcess_Triangulate      |
        aiProcess_GenSmoothNormals |
        aiProcess_GenUVCoords      |
        aiProcess_SortByPType
        );

    if(scene)
    {
        textureArray.resize(scene->mNumMaterials);

        std::string pathString(path);
        size_t last_index = pathString.find_last_of("/\\");
        pathString = pathString.substr(0, last_index + 1);


        for(unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            aiMaterial *currentMaterial = scene->mMaterials[i];

            aiString texturePath;
            aiGetMaterialTexture(currentMaterial, aiTextureType_DIFFUSE, 0, &texturePath, 0, 0, 0, 0, 0, 0);

            const std::string fullTexturePath(pathString + texturePath.data);
            textureArray[i].load(fullTexturePath.data());
        }

        return loadScene(scene);
    }

    return false;
}

bool AnimatedModel::loadScene(const aiScene *scene)
{
    meshArray.resize(scene->mNumMeshes);

    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *currentMesh = scene->mMeshes[i];

        meshArray[i].loadMesh(currentMesh);

        for(unsigned int j = 0; j < currentMesh->mNumBones; j++)
        {
            aiBone *currentBone = currentMesh->mBones[j];

            const std::string name(currentBone->mName.data);

            nodesByName[name] = scene->mRootNode->FindNode(currentBone->mName);
        }
    }

    if(scene->HasAnimations() == true)
    {
        aiAnimation *currentAnimation = scene->mAnimations[0];

        for(unsigned int i = 0; i < currentAnimation->mNumChannels; i++)
        {
            aiNodeAnim *currentChannel = currentAnimation->mChannels[i];
            const std::string name(currentChannel->mNodeName.data);
            channelsByName[name] = currentChannel;
        }
    }

    return scene->mNumMeshes != 0;
}

void AnimatedModel::draw(const Graphics &graphics, double timeElapsed)
{
/*
    if(animationRange)
    {std::cout << "Capping range!" << std::endl;
        if(timeElapsed < animationRange->first)
            timeElapsed = animationRange->first;
        else if(timeElapsed > animationRange->second)
            timeElapsed = fmod(timeElapsed, animationRange->second);
    }
*/


    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *currentMesh = scene->mMeshes[i];

        if(strcmp("Bounds", currentMesh->mName.data) == 0)
        {
            continue;
        }

        std::vector<glm::mat4x4> boneMatrices = calculateBoneMatrices(currentMesh, timeElapsed);
        glUniformMatrix4fv(graphics.uniformBoneMatricesAnim, 64, GL_FALSE, (GLfloat*)&boneMatrices[0]);
        textureArray[currentMesh->mMaterialIndex].bind();
        meshArray[i].draw(graphics, timeElapsed);
    }
}

static glm::vec3 toVec3(const aiVector3D &vector)
{
    return glm::vec3(vector.x, vector.y, vector.z);
}

static glm::mat4x4 toMatrix(const aiMatrix4x4 &matrix)
{
    glm::mat4x4 result;
    result[0][0] = matrix.a1;
    result[0][1] = matrix.b1;
    result[0][2] = matrix.c1;
    result[0][3] = matrix.d1;
    result[1][0] = matrix.a2;
    result[1][1] = matrix.b2;
    result[1][2] = matrix.c2;
    result[1][3] = matrix.d2;
    result[2][0] = matrix.a3;
    result[2][1] = matrix.b3;
    result[2][2] = matrix.c3;
    result[2][3] = matrix.d3;
    result[3][0] = matrix.a4;
    result[3][1] = matrix.b4;
    result[3][2] = matrix.c4;
    result[3][3] = matrix.d4;
    return result;
}

static glm::vec3 interpolatePosition(const aiAnimation *animation, const aiNodeAnim *channel, double currentTime, double minimumTime, double maximumTime)
{

    unsigned int minimumFrame = 0;

    while(minimumFrame < channel->mNumPositionKeys - 1 && minimumTime > channel->mPositionKeys[minimumFrame + 1].mTime)
    {
        minimumFrame++;
    }

    currentTime = minimumTime + fmod(currentTime, maximumTime - minimumTime);

    unsigned int maximumFrame = minimumFrame;

    while(maximumFrame < channel->mNumPositionKeys - 1 && maximumTime > channel->mPositionKeys[maximumFrame + 1].mTime)
    {
        maximumFrame++;
    }

    currentTime = minimumTime + fmod(currentTime, maximumTime - minimumTime);

    unsigned int currentFrame = minimumFrame;

    while(currentFrame < channel->mNumPositionKeys - 1 && currentTime > channel->mPositionKeys[currentFrame + 1].mTime)
    {
        currentFrame++;
    }

    const unsigned int nextFrame = currentFrame == maximumFrame ? minimumFrame : currentFrame + 1;

    const aiVectorKey &currentKey = channel->mPositionKeys[currentFrame];
    const aiVectorKey &nextKey = channel->mPositionKeys[nextFrame];

    double timeDifference = nextKey.mTime - currentKey.mTime;
    if(timeDifference < 0)
        timeDifference += maximumTime - minimumTime;

    const double interpolationFactor = timeDifference ? (currentTime - currentKey.mTime) / timeDifference : 0.0f;

    return toVec3(currentKey.mValue) + toVec3(nextKey.mValue - currentKey.mValue) * (float)interpolationFactor;
}

static glm::mat4x4 interpolateRotation(const aiAnimation *animation, const aiNodeAnim *channel, double currentTime, double minimumTime, double maximumTime)
{
    unsigned int minimumFrame = 0;

    while(minimumFrame < channel->mNumRotationKeys - 1 && minimumTime > channel->mRotationKeys[minimumFrame + 1].mTime)
    {
        minimumFrame++;
    }

    currentTime = minimumTime + fmod(currentTime, maximumTime - minimumTime);

    unsigned int maximumFrame = minimumFrame;

    while(maximumFrame < channel->mNumRotationKeys - 1 && maximumTime > channel->mRotationKeys[maximumFrame + 1].mTime)
    {
        maximumFrame++;
    }

    currentTime = minimumTime + fmod(currentTime, maximumTime - minimumTime);

    unsigned int currentFrame = minimumFrame;

    while(currentFrame < channel->mNumRotationKeys - 1 && currentTime > channel->mRotationKeys[currentFrame + 1].mTime)
    {
        currentFrame++;
    }

    const unsigned int nextFrame = currentFrame == maximumFrame ? minimumFrame : currentFrame + 1;

    const aiQuatKey &currentKey = channel->mRotationKeys[currentFrame];
    const aiQuatKey &nextKey = channel->mRotationKeys[nextFrame];

    double timeDifference = nextKey.mTime - currentKey.mTime;
    if(timeDifference < 0)
        timeDifference += maximumTime - minimumTime;

    const double interpolationFactor = timeDifference ? (currentTime - currentKey.mTime) / timeDifference : 0.0f;

    const aiQuaternion &currentQuat = currentKey.mValue;
    const aiQuaternion &nextQuat = nextKey.mValue;

    aiQuaternion result;
    aiQuaternion::Interpolate(result, currentQuat, nextQuat, (float)interpolationFactor);

    return toMatrix(aiMatrix4x4(result.GetMatrix()));
}


static glm::vec3 interpolateScale(const aiAnimation *animation, const aiNodeAnim *channel, double currentTime, double minimumTime, double maximumTime)
{

    unsigned int minimumFrame = 0;

    while(minimumFrame < channel->mNumScalingKeys - 1 && minimumTime > channel->mScalingKeys[minimumFrame + 1].mTime)
    {
        minimumFrame++;
    }

    currentTime = minimumTime + fmod(currentTime, maximumTime - minimumTime);

    unsigned int currentFrame = minimumFrame;

    while(currentFrame < channel->mNumScalingKeys - 1 && currentTime > channel->mScalingKeys[currentFrame + 1].mTime)
    {
        currentFrame++;
    }

    const unsigned int nextFrame = channel->mScalingKeys[currentFrame + 1].mTime < maximumTime ? (currentFrame + 1) : minimumFrame;

    const aiVectorKey &currentKey = channel->mScalingKeys[currentFrame];
    const aiVectorKey &nextKey = channel->mScalingKeys[nextFrame];

    double timeDifference = nextKey.mTime - currentKey.mTime;

    if(timeDifference < 0)
    {
        timeDifference += animation->mDuration;
    }

    const double interpolationFactor = timeDifference ? (currentTime - currentKey.mTime) / timeDifference : 0.0f;

    return toVec3(currentKey.mValue) + toVec3(nextKey.mValue - currentKey.mValue) * (float)interpolationFactor;
}

glm::mat4x4 AnimatedModel::getNodeTransform(const aiNode *node, double timeElapsed)
{
    const std::string name(node->mName.data);

    std::map<std::string, aiNodeAnim*>::iterator channelIterator = channelsByName.find(name);

    if(channelIterator == channelsByName.end())
    {
        // std::cerr << "No channel found for node: " << name << std::endl;
        return node->mParent ? getNodeTransform(node->mParent, timeElapsed) : glm::mat4x4();
    }

    const aiNodeAnim *currentChannel = channelIterator->second;

    const glm::mat4x4 channelRotation = interpolateRotation(scene->mAnimations[0], currentChannel, timeElapsed, animationRange->first, animationRange->second);
    const glm::mat4x4 channelScale = glm::scale(glm::mat4x4(), interpolateScale(scene->mAnimations[0], currentChannel, timeElapsed, animationRange->first, animationRange->second));
    const glm::mat4x4 channelPosition = glm::translate(glm::mat4x4(), interpolatePosition(scene->mAnimations[0], currentChannel, timeElapsed, animationRange->first, animationRange->second)/*toVec3(currentChannel->mPositionKeys[0].mValue)*/);

    const glm::mat4x4 channelTransform = channelPosition * channelRotation /* * channelScale*/;

    if(node->mParent)
    {
        return getNodeTransform(node->mParent, timeElapsed) * channelTransform;
    }

    return channelTransform;
}

std::vector<glm::mat4x4> AnimatedModel::calculateBoneMatrices(const aiMesh* mesh, double timeElapsed)
{
    std::vector<glm::mat4x4> boneMatrices(64);

    for(unsigned int i = 0; i < mesh->mNumBones && i < 64; i++)
    {
        const aiBone *currentBone = mesh->mBones[i];
        const std::string name(currentBone->mName.data);
        const aiNode *currentNode = nodesByName[name];
        const glm::mat4x4 boneTransform = getNodeTransform(currentNode, timeElapsed) * toMatrix(currentBone->mOffsetMatrix);
        boneMatrices[i] = boneTransform;
    }

    return boneMatrices;
}

AnimatedMesh::AnimatedMesh()
{
    glGenBuffers(6, &buffer[0]);
}

AnimatedMesh::~AnimatedMesh()
{
    glDeleteBuffers(6, &buffer[0]);
}

bool AnimatedMesh::loadMesh(const aiMesh *mesh)
{
    if(!mesh->mNumVertices || !mesh->mNumFaces)
    {
        std::cerr << "The mesh does not contain necessary data. Skipping!" << std::endl;
        return false;
    }

    std::vector<glm::vec3> positionArray(mesh->mNumVertices);
    std::vector<glm::vec3> normalArray(mesh->mNumVertices);
    std::vector<glm::vec2> textureCoordArray(mesh->mNumVertices);
    std::vector<glm::uvec4> boneIndexArray(mesh->mNumVertices);
    std::vector<glm::vec4> boneWeightArray(mesh->mNumVertices);
    std::vector<GLuint> vertexIndexArray;

    for(unsigned int j = 0; j < mesh->mNumVertices; j++)
    {
        if(mesh->HasPositions() == true)
        {
            const aiVector3D *currentPosition = mesh->mVertices + j;
            positionArray[j] = glm::vec3(currentPosition->x, currentPosition->y, currentPosition->z);
        }

        if(mesh->HasNormals() == true)
        {
            const aiVector3D *currentNormal = mesh->mNormals + j;
            normalArray[j] = glm::vec3(currentNormal->x, currentNormal->y, currentNormal->z);
        }

        if(mesh->HasTextureCoords(0) == true)
        {
            const aiVector3D *currentTextureCoord = *mesh->mTextureCoords + j;
            textureCoordArray[j] = glm::vec2(currentTextureCoord->x, 1.0f - currentTextureCoord->y);
        }
    }

    for(unsigned int j = 0; j < mesh->mNumFaces; j++)
    {
        aiFace *currentFace = mesh->mFaces + j;

        for(unsigned int k = 0; k < currentFace->mNumIndices; k++)
        {
            vertexIndexArray.emplace_back(currentFace->mIndices[k]);
        }
    }

    std::vector<std::vector<GLuint>> boneIndicesByVertex(mesh->mNumVertices);
    std::vector<std::vector<GLfloat>> boneWeightsByVertex(mesh->mNumVertices);

    for(unsigned int j = 0; j < mesh->mNumBones; j++)
    {
        aiBone *currentBone = mesh->mBones[j];

        if(!currentBone)
        {
            continue;
        }

        for(unsigned int k = 0; k < currentBone->mNumWeights; k++)
        {
            aiVertexWeight *currentWeight = currentBone->mWeights + k;
            boneIndicesByVertex[currentWeight->mVertexId].emplace_back(j);
            boneWeightsByVertex[currentWeight->mVertexId].emplace_back(currentWeight->mWeight);
        }

    }

    for(unsigned int j = 0; j < mesh->mNumVertices; j++)
    {
        std::vector<GLuint> &indexArray = boneIndicesByVertex[j];

        boneIndexArray[j] = glm::uvec4(0, 0, 0, 0);

        if(indexArray.size() > 4)
        {
            std::cerr << "Warning: More than 4 bone indices per vertex." << std::endl;
        }

        for(size_t k = 0; k < indexArray.size() && k < 4; k++)
        {
            boneIndexArray[j][k] = indexArray[k];
        }

        std::vector<GLfloat> &weightArray = boneWeightsByVertex[j];

        boneWeightArray[j] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        if(weightArray.size() > 4)
        {
            std::cerr << "Warning: More than 4 bone indices per vertex." << std::endl;
        }

        for(size_t k = 0; k < weightArray.size() && k < 4; k++)
        {
            boneWeightArray[j][k] = weightArray[k];
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer[POSITION_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, positionArray.size() * sizeof(glm::vec3), positionArray.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[NORMAL_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, normalArray.size() * sizeof(glm::vec3), normalArray.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[TEXCOORD_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, textureCoordArray.size() * sizeof(glm::vec2), textureCoordArray.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[BONE_INDEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, boneIndexArray.size() * sizeof(glm::uvec4), boneIndexArray.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[BONE_WEIGHT_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, boneWeightArray.size() * sizeof(glm::vec4), boneWeightArray.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndexArray.size() * sizeof(GLuint), vertexIndexArray.data(), GL_STATIC_DRAW);

    numberOfIndices = vertexIndexArray.size();

    return true;
}

void AnimatedMesh::draw(const Graphics &graphics, double timeElapsed) const
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer[POSITION_BUFFER]);
    glVertexAttribPointer(graphics.attributePositionAnim, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[NORMAL_BUFFER]);
    glVertexAttribPointer(graphics.attributeNormalAnim, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[TEXCOORD_BUFFER]);
    glVertexAttribPointer(graphics.attributeTexCoordAnim, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[BONE_INDEX_BUFFER]);
    glVertexAttribPointer(graphics.attributeBoneIndicesAnim, 4, GL_UNSIGNED_INT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[BONE_WEIGHT_BUFFER]);
    glVertexAttribPointer(graphics.attributeBoneWeightsAnim, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, (void*)0);
}

#undef POSITION_BUFFER
#undef NORMAL_BUFFER
#undef TEXCOORD_BUFFER
#undef BONE_INDEX_BUFFER
#undef BONE_WEIGHT_BUFFER
#undef INDEX_BUFFER
