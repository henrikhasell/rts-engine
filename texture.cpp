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

Texture::Texture() : w(0), h(0)
{
    glGenTextures(1, &texture);
/*
    glBindTexture(GL_TEXTURE_2D, texture);

    const GLubyte pixels[] = {
        0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff
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
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}

static inline Uint8 *getPixel(SDL_Surface *surface, int x, int y)
{
    return (Uint8*)surface->pixels + (surface->pitch * y) + (x * surface->format->BytesPerPixel);
}

void Texture::loadSpriteSheet(std::vector<Texture> &container, const char path[])
{
    SDL_Surface *spriteSheet = IMG_Load(path);

    if(spriteSheet)
    {
        SDL_Surface *optimised = SDL_CreateRGBSurface(0, spriteSheet->w, spriteSheet->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);

        if(optimised)
        {
            SDL_BlitSurface(spriteSheet, NULL, optimised, NULL);

            SDL_Rect area;
            area.w = 0;
            area.h = 1;

            size_t numberOfShapes = 0;

            bool *visited = new bool[optimised->w * optimised->h];

            for(int i = 0; i < optimised->w * optimised->h; i++)
            {
                visited[i] = false;
            }

            for(int y = 0; y < optimised->h; y++)
            {
                for(int x = 0; x < optimised->w; x++)
                {
                    Uint8 *pixel = getPixel(optimised, x, y);
                    bool &visited_index = visited[x + y * optimised->w];

                    if(!area.w)
                    {
                        area.x = x;
                        area.y = y;
                    }

                    if(*(Uint32*)pixel == 0xff0000ff || visited_index)
                    {
                        for(int i = 1/* Change to 1? */; i < area.w; i++)
                        {
                                visited_index = true;
                        }

                        for(int j = 0; j < area.h; j++)
                        {
                            for(int i = 0; i < area.w; i++)
                            {
                                Uint8 *pixel = getPixel(optimised, area.x + i, area.y + area.h);
                                bool &visited_index = visited[area.x + i + (area.y + area.h) * optimised->w];

                                if(*(Uint32*)pixel == 0xff0000ff || visited_index)
                                {
                                    break;
                                }
                                else if(i == area.w - 1)
                                {
                                    area.h++;
                                }

                                visited_index = true;
                            }
                        }

                        if(area.w)
                        {
                            std::cout << "Found shape at " <<
                            area.x << ", " <<
                            area.y << " of size " <<
                            area.w << ", " <<
                            area.h << std::endl;

                            SDL_Surface *cropped = SDL_CreateRGBSurface(0, area.w, area.h, 32, R_MASK, G_MASK, B_MASK, A_MASK);

                            if(cropped)
                            {
                                SDL_BlitSurface(optimised, &area, cropped, NULL);

                                container[numberOfShapes++].load(cropped);

                                SDL_FreeSurface(cropped);
                            }


                            area.w = 0;
                            area.h = 1;
                        }
                    }
                    else
                    {
                        area.w++;
                    }
                }
            }

            delete[] visited;

            SDL_FreeSurface(optimised);

            std::cout << "Number of shapes found: " << numberOfShapes << std::endl;
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

        glTexImage2D(GL_TEXTURE_2D, 0, 4,
            optimised->w,
            optimised->h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE,
            optimised->pixels
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        w = optimised->w;
        h = optimised->h;

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

int Texture::getW()
{
    return w;
}

int Texture::getH()
{
    return h;
}
