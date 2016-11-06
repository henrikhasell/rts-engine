#ifndef SCENE_HPP
#define SCENE_HPP

#include <assimp/scene.h>
#include "graphics.hpp"
#include "mesh3d.hpp"

namespace Engine
{
namespace GL
{
class Scene
{
public:
    Scene();
    ~Scene();
    bool load(const char path[]);
    void createMesh(Mesh3D &mesh);
    void createCollisionMesh();
private:
    std::vector<glm::vec3> vertexBuffer;
    std::vector<glm::vec3> normalBuffer;
    std::vector<GLuint> indexBuffer;
};
};
};


#endif // SCENE_HPP
