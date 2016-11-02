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
        font = TTF_OpenFontIndexRW(file, 1, 24, 0);
    }

    return font != nullptr;
}

bool Font::renderString(Mesh2D &mesh, const char string[])
{
    SDL_Color white = {255, 255, 255};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, string, white);

    if(surface)
    {
        float w = surface->w;
        float h = surface->h;

        std::vector<glm::vec2> vertexArray {
            {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h}
        };
        std::vector<glm::vec2> uvArray {
            {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}
        };
        std::vector<GLuint> indexArray {
            1, 0, 2, 1, 2, 3
        };

        mesh.setVertices(vertexArray);
        mesh.setTextureCoordinates(uvArray);
        mesh.setIndices(indexArray);
        mesh.setTexture(surface);

        SDL_FreeSurface(surface);
    }
    else
    {
        std::cerr << "Failed to rasterize text: " << TTF_GetError() << std::endl;
    }

    return surface != nullptr;
}
