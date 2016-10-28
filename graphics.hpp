#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "shader.hpp"
#include "program.hpp"

#include <glm/mat4x4.hpp>

namespace Engine
{
namespace GL
{
class Graphics
{
public:
    Graphics();
    bool initialise();
private:
    Shader fragmentShader;
    Shader vertexShader;
    Program program;

    glm::mat4x4 matrixP;
    glm::mat4x4 matrixV;
    glm::mat4x4 matrixM;

    GLuint uniformP;
    GLuint uniformV;
    GLuint uniformM;
}; // Graphics
}; // GL
}; // Engine


#endif // GRAPHICS_HPP
