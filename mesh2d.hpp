#ifndef MESH2D_HPP
#define MESH2D_HPP

#include <vector>
#include <glm/vec2.hpp>
#include <GL/glew.h>

#include "graphics.hpp"

namespace Engine
{
namespace GL
{
class Mesh2D
{
public:
    Mesh2D();
    ~Mesh2D();
    void setVertices(const std::vector<glm::vec2> &vertexArray);
    void setTextureCoordinates(const std::vector<glm::vec2> &uvArray);
    void setIndices(const std::vector<GLuint> &indexArray);
    void draw(const Graphics &graphics);
private:
    std::vector<glm::vec2> vertexArray;
    std::vector<glm::vec2> uvArray;
    std::vector<GLuint> indexArray;
    GLuint buffer[3];
}; // Mesh2D
}; // GL
}; // Engine

#endif // MESH2D_HPP
