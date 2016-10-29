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
    void setVertices(const std::vector<glm::vec4> &vertices);
    void setColours(const std::vector<glm::vec4> &colours);
    void draw(const Graphics &graphics);
private:
    std::vector<glm::vec4> vertex;
    std::vector<glm::vec4> colour;

    GLuint vertexBuffer;
    GLuint colourBuffer;
}; // Mesh
}; // GL
}; // Engine

#endif // MESH_HPP
