#ifndef ANIMATED_MESH_HPP
#define ANIMATED_MESH_HPP

#include <vector>

#include <assimp/scene.h>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>

#include "graphics.hpp"

namespace Engine
{
namespace GL
{
class AnimatedMesh
{
public:
    AnimatedMesh();
    ~AnimatedMesh();
    bool loadScene(const aiScene *scene);
private:
    std::vector<glm::vec3> vertexPosition;
    std::vector<glm::vec3> vertexNormal;
    std::vector<glm::vec2> vertexTexCoord;
    std::vector<glm::vec4> vertexColour;
    std::vector<glm::vec4> vertexBoneWeights;
    std::vector<glm::uvec4> vertexBoneIndices;
    GLuint buffer[6];
}; // AnimatedMesh
}; // GL
}; // Engine

#endif // ANIMATED_MESH_HPP
