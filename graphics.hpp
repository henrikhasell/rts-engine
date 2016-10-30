#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "shader.hpp"
#include "program.hpp"

#include <SDL2/SDL.h>
#include <glm/mat4x4.hpp>

namespace Engine
{
namespace GL
{
class Graphics
{
friend class Mesh;

public:
    Graphics(SDL_Window *window);
    bool initialise();
private:
    Shader fragmentShader;
    Shader vertexShader;
    Program program;

    glm::mat4x4 matrixP;
    glm::mat4x4 matrixV;
    glm::mat4x4 matrixM;

    GLint uniformP;
    GLint uniformV;
    GLint uniformM;

    GLint attributePosition;
    GLint attributeNormal;
    GLint attributeColour;
}; // Graphics
}; // GL
}; // Engine


#endif // GRAPHICS_HPP
