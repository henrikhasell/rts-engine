#ifndef ANIMATED_MESH_HPP
#define ANIMATED_MESH_HPP

#include <list>
#include <vector>
#include <map>

#include <assimp/scene.h>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>

#include "graphics.hpp"
#include "texture.hpp"

namespace Engine
{
namespace GL
{

class AnimatedModel;
class AnimatedMesh;
class Bone;
class Node;
class VectorKey;
class QuatKey;
class NodeAnimation;
class Animation;

class Bone
{
public:
    Bone(const aiBone *bone);
private:
    std::string name;
    glm::mat4x4 boneTransform;
    std::vector<GLfloat> weights;
};

class Node
{
public:
    Node(const aiNode *node);
private:
    std::string name;
    std::vector<Node*> children;
    Node *parent;
};

class VectorKey
{
public:
    VectorKey(const aiVectorKey *vectorKey);
private:
    glm::vec3 position;
    double time;
};

class QuatKey
{
public:
    QuatKey(const aiQuatKey *quatKey);
private:
    glm::quat rotation;
    double time;
};

class NodeAnimation
{
public:
    NodeAnimation(const aiNodeAnim *nodeAnim);
private:
    std::string name;
    std::vector<VectorKey> positionKeys;
    std::vector<VectorKey> scalingKeys;
    std::vector<QuatKey> rotationKeys;
};

class Animation
{
public:
    Animation(const aiAnimation *animation);
private:
    std::string name;
    std::vector<NodeAnimation> channels;
    double duration;
};

class AnimatedMesh
{
public:
    AnimatedMesh();
    ~AnimatedMesh();
    bool loadMesh(const aiMesh *mesh);
    void draw(const Graphics &graphics) const;
private:
    GLuint buffer[6];
    GLuint numberOfIndices;
}; // AnimatedMesh

class AnimatedModel
{
public:
    AnimatedModel();
    ~AnimatedModel();
    bool loadScene(const aiScene *scene);
    bool loadFile(const char path[]);
    void draw(const Graphics &graphics, double timeElapsed);
    void setTexture(size_t index, const char path[]);
    bool setAnimationRange(const char name[]);
    void addAnimationRange(const char name[], double start, double finish);
private:
    std::map<std::string, std::pair<double, double>> animationRanges;
    std::vector<glm::mat4x4> calculateBoneMatrices(const aiMesh* mesh, double timeElapsed);
    glm::mat4x4 getNodeTransform(const aiNode *node, double timeElapsed);
    std::map<std::string, aiNode*> nodesByName;
    std::map<std::string, aiNodeAnim*> channelsByName;
    std::vector<AnimatedMesh> meshArray;
    std::vector<Texture> textureArray;
    std::pair<double, double> *animationRange;
    const aiScene *scene;
}; // AnimatedModel
}; // GL
}; // Engine

class ModelAnimator
{

}; // ModelAnimator

#endif // ANIMATED_MESH_HPP
