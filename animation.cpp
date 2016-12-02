#include "animation.hpp"
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <glm/detail/func_matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>

using namespace Engine;
using namespace GL;

#define POSITION_BUFFER 0
#define NORMAL_BUFFER 1
#define TEXCOORD_BUFFER 2
#define BONE_INDEX_BUFFER 3
#define BONE_WEIGHT_BUFFER 4
#define INDEX_BUFFER 5

AnimatedModel::AnimatedModel()
{

}

AnimatedModel::~AnimatedModel()
{

}

bool AnimatedModel::loadFile(const char path[])
{

    scene = importer.ReadFile(path,
        aiProcess_Triangulate      |
        aiProcess_GenSmoothNormals |
        aiProcess_GenUVCoords      |
        aiProcess_SortByPType
        );

    if(scene)
    {
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

    aiAnimation *currentAnimation = scene->mAnimations[0];

    for(unsigned int i = 0; i < currentAnimation->mNumChannels; i++)
    {
        aiNodeAnim *currentChannel = currentAnimation->mChannels[i];

        const std::string name(currentChannel->mNodeName.data);

        std::cout << "Loading channel: " << name << std::endl;

        channelsByName[name] = currentChannel;
    }

    return scene->mNumMeshes != 0;
}

void AnimatedModel::draw(const Graphics &graphics)
{

    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *currentMesh = scene->mMeshes[i];

        std::vector<glm::mat4x4> boneMatrices = calculateBoneMatrices(scene->mRootNode, currentMesh);
        glUniformMatrix4fv(graphics.uniformBoneMatricesAnim, 60, GL_FALSE, (GLfloat*)&boneMatrices[0]);
        meshArray[i].draw(graphics);
    }
}

static glm::vec3 toVec3(const aiVector3D &vector)
{
    return glm::vec3(vector.x, vector.y, vector.z);
}

static glm::quat toQuat(const aiQuaternion &quaternion)
{
    return glm::quat(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
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

static glm::mat4x4 getGlobalTransform(const aiNode *node)
{
    glm::mat4x4 result = toMatrix(node->mTransformation);
/*
    while((node = node->mParent))
    {
        result = toMatrix(node->mTransformation);
    }
*/
    while(node->mParent)
    {
        result = toMatrix(node->mParent->mTransformation);
        node = node->mParent;
    }
    return result;
}

std::vector<glm::mat4x4> AnimatedModel::calculateBoneMatrices(const aiNode *node, const aiMesh* mesh)
{
    std::vector<glm::mat4x4> boneMatrices(mesh->mNumBones);

    static unsigned int frame = 0;

    for(unsigned int i = 0; i < mesh->mNumBones; i++)
    {
        const aiBone *currentBone = mesh->mBones[i];

        const std::string name(currentBone->mName.data);

        const aiNode *currentNode = nodesByName[name];
        const aiNodeAnim *currentChannel = channelsByName[name];

        const glm::vec3 channelPosition = toVec3(currentChannel->mPositionKeys[frame].mValue);
        const glm::quat channelRotation = toQuat(currentChannel->mRotationKeys[frame].mValue);
        const glm::vec3 channelScale = toVec3(currentChannel->mScalingKeys[frame].mValue);

        const glm::mat4x4 globalTransform = getGlobalTransform(currentNode);
        const glm::mat4x4 inverseTransform = glm::inverse(globalTransform);
        const glm::mat4x4 boneTransform = toMatrix(currentBone->mOffsetMatrix);

        const glm::mat4x4 channelTransform =
            glm::translate(
                glm::mat4x4(), channelPosition
            ) *
            glm::rotate(
                glm::mat4x4(), channelRotation.w, glm::vec3(channelRotation.x, channelRotation.y, channelRotation.z)
            ) *
            glm::scale(
                glm::mat4x4(), channelScale
            );

        boneMatrices[i] = inverseTransform * globalTransform * boneTransform;///*inverseTransform * */globalTransform * channelTransform/* * boneTransform*/;
        std::cout << "Scale: " << channelScale.x << ", " << channelScale.y << ", " << channelScale.z << std::endl;
    }

SDL_Delay(100);
    frame++;

    if(frame >= scene->mAnimations[0]->mNumChannels)
        frame = 0;

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

    for(unsigned int j = 0; j < mesh->mNumVertices; j++)
    {
        const aiVector3D *currentPosition = mesh->mVertices + j;
        const aiVector3D *currentNormal = mesh->mNormals + j;
        const aiVector3D *currentTextureCoord = *mesh->mTextureCoords + j;

        positionArray[j] = glm::vec3(currentPosition->x, currentPosition->y, currentPosition->z);
        normalArray[j] = glm::vec3(currentNormal->x, currentNormal->y, currentNormal->z);
        textureCoordArray[j] = glm::vec2(currentTextureCoord->x, 1.0f - currentTextureCoord->y);
    }

    std::vector<GLuint> vertexIndexArray;

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

        for(size_t k = 0; k < indexArray.size(); k++)
        {
            boneIndexArray[j][k] = indexArray[k];
        }

        std::vector<GLfloat> &weightArray = boneWeightsByVertex[j];

        boneWeightArray[j] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        for(size_t k = 0; k < weightArray.size(); k++)
        {
            boneWeightArray[j][k] = weightArray[k];
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer[POSITION_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, positionArray.size() * sizeof(glm::vec3), positionArray.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[NORMAL_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, normalArray.size() * sizeof(glm::vec3), normalArray.data(), GL_STATIC_DRAW);
/*
    glBindBuffer(GL_ARRAY_BUFFER, buffer[TEXCOORD_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, textureCoordArray.size() * sizeof(glm::vec2), textureCoordArray.data(), GL_STATIC_DRAW);
*/
    glBindBuffer(GL_ARRAY_BUFFER, buffer[BONE_INDEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, boneIndexArray.size() * sizeof(glm::uvec4), boneIndexArray.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[BONE_WEIGHT_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, boneWeightArray.size() * sizeof(glm::vec4), boneWeightArray.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndexArray.size() * sizeof(GLuint), vertexIndexArray.data(), GL_STATIC_DRAW);

    numberOfIndices = vertexIndexArray.size();

    return true;
}

void AnimatedMesh::draw(const Graphics &graphics)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer[POSITION_BUFFER]);
    glVertexAttribPointer(graphics.attributePositionAnim, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[NORMAL_BUFFER]);
    glVertexAttribPointer(graphics.attributeNormalAnim, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
/*
    glBindBuffer(GL_ARRAY_BUFFER, buffer[TEXCOORD_BUFFER]);
    glVertexAttribPointer(graphics.attributeTexCoordAnim, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
*/
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
