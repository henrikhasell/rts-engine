#ifndef FONT_HPP
#define FONT_HPP

#include "texture.hpp"
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
    Texture texture;
    Mesh2D mesh;
};

class Font
{
    public:
        Font();
        ~Font();
        bool load(const char path[]);
        Text renderString(const char string[]);
    private:
        TTF_Font *font;
};
}; // TTF
}; // GL
}; // Engine

#endif // FONT_HPP
