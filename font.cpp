#include "font.hpp"
#include <iostream>

using namespace Engine;
using namespace GL;
using namespace TTF;

Font::Font() : font(nullptr)
{
    //ctor
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
        font = TTF_OpenFontIndexRW(file, 1, 128, 0);
    }

    return font != nullptr;
}

Surface Font::renderString(const char string[])
{
    SDL_Color white = {255, 255, 255};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, string, white);

    if(!surface)
    {
        std::cerr << "Failed to rasterize font." << std::endl;
    }

    return Surface(surface);
}
