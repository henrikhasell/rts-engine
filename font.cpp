#include "font.hpp"
#include <iostream>
#include <vector>

using namespace Engine;
using namespace GL;

Font::Font() : font(nullptr)
{
}

Font::~Font()
{
    if(font) TTF_CloseFont(font);
}

bool Font::load(const char path[])
{
    SDL_RWops * file = SDL_RWFromFile(path, "r");

    if(file)
    {
        font = TTF_OpenFontIndexRW(file, 1, FONT_SIZE, 0);
    }

    return font != nullptr;
}

bool Font::renderString(Mesh2D &mesh, Texture &texture, const char string[]) const
{
    SDL_Color white = {255, 255, 255};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, string, white);

    if(surface)
    {
        float w = surface->w;
        float h = surface->h;

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

