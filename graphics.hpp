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
friend class Mesh3D;
friend class Mesh2D;
friend class Texture;
friend class Camera;

public:
    Graphics(SDL_Window *window);
    bool initialise();
    void begin2D();
    void begin3D();
    void end2D();
    void end3D();
    void translate3D(const glm::vec3 &position);
protected:
    bool initialise2D();
    bool initialise3D();
private:
    Shader fragmentShader3D;
    Shader vertexShader3D;
    Program program3D;

    glm::mat4x4 matrixP3D;
    glm::mat4x4 matrixV3D;
    glm::mat4x4 matrixM3D;

    GLint uniformP3D;
    GLint uniformV3D;
    GLint uniformM3D;
    GLint uniformViewPosition3D;

    GLint attributePosition3D;
    GLint attributeNormal3D;

    Shader fragmentShader2D;
    Shader vertexShader2D;
    Program program2D;

    glm::mat4x4 matrixP2D;
    glm::mat4x4 matrixV2D;
    glm::mat4x4 matrixM2D;

    GLint uniformTextureSampler2D;
    GLint uniformP2D;
    GLint uniformV2D;
    GLint uniformM2D;

    GLint attributePosition2D;
    GLint attributeUV2D;
}; // Graphics
}; // GL
}; // Engine


#endif // GRAPHICS_HPP
