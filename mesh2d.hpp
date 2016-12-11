#ifndef MESH2D_HPP
#define MESH2D_HPP

#include <vector>
#include <glm/vec2.hpp>
#include <GL/glew.h>

#include "graphics.hpp"
#include "texture.hpp"

namespace Engine
{
namespace GL
{
class Mesh2D
{
public:
    static void createRectangle(Mesh2D &mesh, float x, float y, float w, float h);
    Mesh2D();
    ~Mesh2D();
    void setVertices(const std::vector<glm::vec2> &vertexArray);
    void setTextureCoordinates(const std::vector<glm::vec2> &uvArray);
    void setIndices(const std::vector<GLuint> &indexArray);
    void draw(const Graphics &graphics, const glm::vec2 &position) const;
    void draw(const Graphics &graphics) const;
private:
    GLuint buffer[3];
    GLuint numberOfIndices;
}; // Mesh2D
}; // GL
}; // Engine

#endif // MESH2D_HPP
