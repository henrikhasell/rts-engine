#ifndef FONT_HPP
#define FONT_HPP

#include "mesh2d.hpp"
#include <SDL2/SDL_ttf.h>

#define FONT_SIZE 32

namespace Engine
{
namespace GL
{
class Font
{
    public:
        Font();
        ~Font();
        bool load(const char path[]);
        bool renderString(Mesh2D &mesh, const char string[]) const;
    private:
        TTF_Font *font;
}; // Font
}; // GL

}; // Engine

#endif // FONT_HPP
