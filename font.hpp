#ifndef FONT_HPP
#define FONT_HPP

#include "surface.hpp"
#include "mesh2d.hpp"
#include <SDL2/SDL_ttf.h>

namespace Engine
{
namespace GL
{
namespace TTF
{

struct Text
{
    GLuint texture;
    Mesh2D mesh;
};

class Font
{
    public:
        Font();
        bool load(const char path[]);
        GLuint renderString(const char string[]);
        ~Font();
    private:
        TTF_Font *font;
};
}; // TTF
}; // GL
}; // Engine

#endif // FONT_HPP
