#include "texture.hpp"
#include <iostream>

using namespace Engine;
using namespace GL;

Texture::Texture()
{
    glGenTextures(1, &texture);
    std::cout << "Creating texture " << texture << " at address " << this << std::endl;
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
    std::cout << "Destroying texture " << texture << " at address " << this << std::endl;
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

/* Test code:

    float pixels[] = {
        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
    };

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return true;
*/

bool Texture::load(SDL_Surface *surface)
{
    SDL_Surface *optimised = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);

    if(optimised)
    {
        SDL_BlitSurface(surface, NULL, optimised, NULL);

        glBindTexture(GL_TEXTURE_2D, texture);

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

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}
