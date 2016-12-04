#include "texture.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>

using namespace Engine;
using namespace GL;

Texture::Texture()
{
    glGenTextures(1, &texture);
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#   define R_MASK 0xFF000000
#   define G_MASK 0x00FF0000
#   define B_MASK 0x0000FF00
#   define A_MASK 0x000000FF
#else
#   define R_MASK 0x000000FF
#   define G_MASK 0x0000FF00
#   define B_MASK 0x00FF0000
#   define A_MASK 0xFF000000
#endif // SDL_BYTEORDER

bool Texture::load(const char path[])
{
    bool result = false;

    SDL_Surface *surface = IMG_Load(path);

    if(surface)
    {
        result = load(surface);
    }

    SDL_FreeSurface(surface);

    std::cout << "Loading " << path << ": " << result << std::endl;

    return result;
}

bool Texture::load(SDL_Surface *surface)
{
    SDL_Surface *optimised = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);

    if(optimised)
    {
        SDL_BlitSurface(surface, NULL, optimised, NULL);

        glBindTexture(GL_TEXTURE_2D, texture);
/*
        GLubyte pixels[] = {
            0xff, 0xff, 0xff, 0xff,   0xff, 0xff, 0xff, 0xff,
            0xff, 0xff, 0xff, 0xff,   0xff, 0xff, 0xff, 0xff
        };
        glTexImage2D(GL_TEXTURE_2D, 0, 4,
            2,
            2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE,
            pixels
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
*/
        glTexImage2D(GL_TEXTURE_2D, 0, 4,
            optimised->w,
            optimised->h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE,
            optimised->pixels
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SDL_FreeSurface(optimised);
    }

    return optimised != NULL;
}

#undef R_MASK
#undef G_MASK
#undef B_MASK
#undef A_MASK

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, texture);
}
