#include "heightmap.hpp"

using namespace Engine;
using namespace GL;

Heightmap::Heightmap() : height(nullptr), w(0), h(0)
{

}

Heightmap::~Heightmap()
{
    if(height)
    {
        delete[] height;
    }
}

void Heightmap::draw(const Graphics &graphics) const
{
    mesh.draw(graphics);
}

bool Heightmap::load(const char path[])
{
    SDL_Surface *surface = SDL_LoadBMP(path);
    SDL_Surface *optimised = nullptr;

    if(surface)
    {
        optimised = SDL_CreateRGBSurface(0, surface->w, surface->h, 24, 0, 0, 0, 0);

        if(optimised)
        {
            SDL_BlitSurface(surface, NULL, optimised, NULL);

            height = new float[optimised->w * optimised->h];

            w = optimised->w;
            h = optimised->h;

            for(int y = 0; y < h; y++)
            {
                for(int x = 0; x < w; x++)
                {
                    Uint8 *pixel = (Uint8*)surface->pixels + (surface->pitch * y) + (x * surface->format->BytesPerPixel);
                    height[x + y * w] = (float)(pixel[0] + pixel[1] + pixel[2]) / 3.0f;
                }
            }

            SDL_FreeSurface(optimised);
        }

        SDL_FreeSurface(surface);
    }

    return optimised != nullptr;
}

void Heightmap::generateMesh()
{
    std::vector<glm::vec3> vertexArray;
    std::vector<GLuint> indexArray;

    for(int z = 0; z < h; z++)
    {
        for(int x = 0; x < w; x++)
        {
            const GLfloat vx = (GLfloat)(HEIGHTMAP_TILE_W * x);
            const GLfloat vy = (GLfloat)height[x + z * w];
            const GLfloat vz = (GLfloat)(HEIGHTMAP_TILE_H * z);

            vertexArray.emplace_back(vx, vy, vz);

        }
    }

    for(int z = 0; z < h - 1; z++)
    {
        for(int x = 0; x < w - 1; x++)
        {
            const GLuint index = z * w + x;

            indexArray.emplace_back(index + 1);
            indexArray.emplace_back(index);
            indexArray.emplace_back(index + w);

            indexArray.emplace_back(index + 1);
            indexArray.emplace_back(index + w);
            indexArray.emplace_back(index + w + 1);
        }
    }

    mesh.setVertices(vertexArray);
    mesh.setIndices(indexArray);
    mesh.calculateNormals();
}
