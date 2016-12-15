#ifndef FONT_HPP
#define FONT_HPP

#include "mesh2d.hpp"
#include <SDL2/SDL_ttf.h>
#include <map>

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
        bool renderString(Mesh2D &mesh, Texture &texture, const char string[], int size, float &w, float &h);
    private:
        std::map<const int, TTF_Font*> fontMap;
        SDL_RWops * file;
}; // Font
}; // GL

}; // Engine

#endif // FONT_HPP
