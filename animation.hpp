#ifndef ANIMATED_MESH_HPP
#define ANIMATED_MESH_HPP

#include <vector>
#include <SDL2/SDL.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "graphics.hpp"

namespace Engine
{
namespace GL
{
/// TODO: Add bones.
struct Transformation
{
    /// TODO: Add reference to bone.
    std::vector<glm::vec3> position;
    std::vector<glm::quat> rotation;
}; // Transformation
struct AnimationInfo
{
    std::string name;
    unsigned int numberOfTicks;
    unsigned int ticksPerSecond;
    std::vector<Transformation> transformations;
}; // AnimationInfo
class AnimatedMesh
{
public:
    AnimatedMesh();
    ~AnimatedMesh();
    bool load(const char path[]);
    void draw(const Graphics &graphics, Uint32 milliseconds);
protected:
    std::vector<AnimationInfo> animationInfo;
    AnimationInfo *currentAnimation;
    /// TODO: Add bones.
}; // AnimatedMesh
}; // GL
}; // Engine

#endif // ANIMATED_MESH_HPP
