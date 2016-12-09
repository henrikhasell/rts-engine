#include "texture.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>

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

static inline Uint8 *getPixel(SDL_Surface *surface, int x, int y)
{
    return (Uint8*)surface->pixels + (surface->pitch * y) + (x * surface->format->BytesPerPixel);
}

void Texture::loadSpriteSheet(std::vector<Texture> &apperance, const char path[])
{
    SDL_Surface *spriteSheet = IMG_Load(path);

    if(spriteSheet)
    {
        SDL_Surface *optimised = SDL_CreateRGBSurface(0, spriteSheet->w, spriteSheet->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);

        if(optimised)
        {
            SDL_BlitSurface(spriteSheet, NULL, optimised, NULL);

            int start_x;
            int start_y;
            int curr_w = 0;
            int curr_h = 1;

            for(int y = 0; y < optimised->h; y++)
            {
                for(int x = 0; x < optimised->w; x++)
                {
                    Uint8 *pixel = getPixel(optimised, x, y);

                    if(!curr_w)
                    {
                        start_x = x;
                        start_y = y;
                    }

                    if(*(Uint32*)pixel == 0xff0000ff)
                    {
                        for(int i = 0; i < curr_w; i++)
                        {
                                Uint8 *pixel = getPixel(optimised, start_x + i, start_y);
                                *(Uint32*)pixel = 0xff0000ff;
                        }

                        for(int j = 0; j < curr_h; j++)
                        {
                            for(int i = 0; i < curr_w; i++)
                            {
                                Uint8 *pixel = getPixel(optimised, start_x + i, start_y + curr_h);

                                if(*(Uint32*)pixel == 0xff0000ff)
                                {
                                    break;
                                }
                                else if(i == curr_w - 1)
                                {
                                    curr_h++;
                                }

                                *(Uint32*)pixel = 0xff0000ff;
                            }
                        }

                        if(curr_w)
                        {
                            std::cout << "Found shape at " << start_x << ", " << start_y << " of size " << curr_w << ", " << curr_h << std::endl;
                            curr_w = 0;
                            curr_h = 1;
                        }
                    }
                    else
                    {
                        curr_w++;
                    }
                }
            }

            SDL_FreeSurface(optimised);
        }

        SDL_FreeSurface(spriteSheet);
    }
}
bool Texture::load(const char path[])
{
    bool result = false;

    SDL_Surface *surface = IMG_Load(path);

    if(surface)
    {
        result = load(surface);
    }

    SDL_FreeSurface(surface);

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
