#ifndef MESH3D_HPP
#define MESH3D_HPP

#include <vector>
#include <glm/vec3.hpp>
#include <GL/glew.h>

#include "graphics.hpp"

namespace Engine
{
namespace GL
{
class Mesh3D
{
public:
    static bool loadHeightMap(Mesh3D &mesh, const char path[]);
    Mesh3D();
    ~Mesh3D();
    void setVertices(const std::vector<glm::vec3> &vertexArray);
    void setNormals(const std::vector<glm::vec3> &normalArray);
    void setIndices(const std::vector<GLuint> &indexArray);
    void draw(const Graphics &graphics, const glm::vec3 &position) const;
    void draw(const Graphics &graphics) const;
    void calculateNormals();
private:
    std::vector<glm::vec3> vertexArray;
    std::vector<glm::vec3> normalArray;
    std::vector<GLuint> indexArray;
    GLuint buffer[3];
}; // Mesh3D
}; // GL
}; // Engine

#endif // MESH3D_HPP
