#ifndef MESH3D_HPP
#define MESH3D_HPP

#include <list>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <GL/glew.h>

#include "graphics.hpp"

namespace Engine
{
namespace GL
{
class Mesh3D
{
public:
    Mesh3D();
    ~Mesh3D();

    bool load(const aiMesh *mesh);

    void setVertices(const std::vector<glm::vec3> &vertexArray);
    void setNormals(const std::vector<glm::vec3> &normalArray);
    void setIndices(const std::vector<GLuint> &indexArray);

    void draw(const Graphics &graphics, const glm::vec3 &position, const glm::quat &rotation) const;
    void draw(const Graphics &graphics, const glm::vec3 &position) const;
    void draw(const Graphics &graphics) const;

    void calculateNormals();
private:
    GLuint buffer[3];
    GLuint numberOfIndices;
}; // Mesh3D

class Model3D
{
public:
    bool loadFile(const char path[]);
    bool loadScene(const aiScene *scene);
    void draw(const Graphics &graphics) const;
private:
    std::list<Mesh3D> meshArray;
}; // Model3D

}; // GL
}; // Engine

#endif // MESH3D_HPP
