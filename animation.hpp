#ifndef ANIMATED_MESH_HPP
#define ANIMATED_MESH_HPP

#include <vector>
#include <map>

#include <SDL2/SDL.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "graphics.hpp"

namespace Engine
{
namespace GL
{
struct VertexWeight
{
    size_t vertexId;
    float weight;
}; // VertexWeight
struct Bone
{
    std::string name;
    Bone *parent;
    std::vector<Bone*> child;
    std::vector<VertexWeight> weight;
    glm::mat4x4 offset;
}; // Bone
struct Channel
{
    std::vector<glm::vec3> position;
    std::vector<glm::quat> rotation;
    std::string name;
}; // Channel
struct Animation
{
    std::string name;
    unsigned int numberOfTicks;
    unsigned int ticksPerSecond;
    std::vector<Channel> channel;
}; // Animation
class AnimatedMesh
{
public:
    AnimatedMesh();
    ~AnimatedMesh();
    bool load(const char path[]);
    void draw(const Graphics &graphics, Uint32 milliseconds);
protected:
    // std::vector<AnimationInfo> animationInfo;
    std::vector<Bone> boneArray;
    // AnimationInfo *currentAnimation;
    /// TODO: Add bones.
}; // AnimatedMesh
}; // GL
}; // Engine

#endif // ANIMATED_MESH_HPP
