#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glm/vec3.hpp>
#include <GL/glew.h>

#include "graphics.hpp"

namespace Engine
{
namespace GL
{
class Mesh
{
friend class Graphics;

public:
    Mesh();
    ~Mesh();

    void setVertices(const std::vector<glm::vec3> &vertexArray);
    void setNormals(const std::vector<glm::vec3> &normalArray);
    void setIndices(const std::vector<GLuint> &indexArray);

    void draw(const Graphics &graphics);
private:
    std::vector<glm::vec3> vertexArray;
    std::vector<glm::vec3> normalArray;
    std::vector<GLuint> indexArray;

    GLuint buffer[3];
}; // Mesh
}; // GL
}; // Engine

#endif // MESH_HPP
