#ifndef FONT_HPP
#define FONT_HPP

#include "surface.hpp"
#include <SDL2/SDL_ttf.h>

namespace Engine
{
namespace GL
{
namespace TTF
{
class Font
{
    public:
        Font();
        bool load(const char path[]);
        Surface renderString(const char string[]);
        ~Font();
    private:
        TTF_Font *font;
};
}; // TTF
}; // GL
}; // Engine

#endif // FONT_HPP
