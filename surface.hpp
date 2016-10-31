#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include "graphics.hpp"

namespace Engine
{
namespace GL
{
class Surface
{
friend class Graphics;

public:
    const GLint width;
    const GLint height;
    Surface(SDL_Surface *surface);
    ~Surface();
    void draw(const Graphics &graphics);
private:
    GLuint texture;
    GLuint buffer[2];
    glm::vec2 vertexArray[4];
    glm::vec2 uvArray[4];
    GLuint indexArray[6];
}; // Surface
}; // GL
}; // Engine

#endif // SURFACE_HPP
