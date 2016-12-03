#ifndef ANIMATED_MESH_HPP
#define ANIMATED_MESH_HPP

#include <list>
#include <vector>
#include <map>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

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
    bool loadMesh(const aiMesh *mesh);
    void draw(const Graphics &graphics);
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
    void draw(const Graphics &graphics);
    std::vector<glm::mat4x4> calculateBoneMatrices(const aiMesh* mesh);
private:
    glm::mat4x4 getNodeTransform(const aiNode *node, double timeElapsed);
    std::vector<AnimatedMesh> meshArray;
    std::map<std::string, aiNode*> nodesByName;
    std::map<std::string, aiNodeAnim*> channelsByName;
    Assimp::Importer importer;
    const aiScene *scene;
}; // AnimatedModel
}; // GL
}; // Engine

class ModelAnimator
{

}; // ModelAnimator

#endif // ANIMATED_MESH_HPP
