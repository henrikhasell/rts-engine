#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <GL/glew.h>

#include "graphics.hpp"

namespace Engine
{
namespace GL
{
class Mesh
{
public:
    Mesh();
    ~Mesh();
    void setVertices(const std::vector<glm::vec4> &vertexArray);
    // void setColours(const std::vector<glm::vec4> &colourArray);
    void setIndices(const std::vector<GLuint> &indexArray);
    void draw(const Graphics &graphics);
private:
    std::vector<glm::vec4> vertexArray;
    // std::vector<glm::vec4> colourArray;
    std::vector<GLuint> indexArray;

    GLuint buffer[3];
}; // Mesh
}; // GL
}; // Engine

#endif // MESH_HPP
