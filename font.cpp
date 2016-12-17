#include "font.hpp"
#include <iostream>
#include <vector>

using namespace Engine;
using namespace GL;

Font::Font() : file(nullptr)
{

}

Font::~Font()
{
    for(std::pair<const int,TTF_Font*> &pair : fontMap)
    {
        TTF_CloseFont(pair.second);
    }
    if(file) SDL_RWclose(file);
}

bool Font::load(const char path[])
{
    file = SDL_RWFromFile(path, "r");

    return file != nullptr;
}

bool Font::renderString(Mesh2D &mesh, Texture &texture, const char string[], int size, float &w, float &h)
{
    std::map<const int,TTF_Font*>::const_iterator i = fontMap.find(size);

    TTF_Font *selected;

    if(i == fontMap.end())
    {
        SDL_RWseek(file, 0, RW_SEEK_SET);
        selected = fontMap[size] = TTF_OpenFontRW(file, 0, size);

    }
    else
    {
        selected = (*i).second;
    }

    SDL_Color white = {0, 0, 0, 255};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(selected, string, white);

    if(surface)
    {
        w = (float)surface->w;
        h = (float)surface->h;

        Mesh2D::createRectangle(mesh, 0.0f, 0.0f, w, h);

        texture.load(surface);

        SDL_FreeSurface(surface);
    }
    else
    {
        std::cerr << "Failed to rasterize text: " << TTF_GetError() << std::endl;
    }

    return surface != nullptr;
}

