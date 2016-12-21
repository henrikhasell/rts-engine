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
                    const Uint8 *pixel = (Uint8*)surface->pixels + (surface->pitch * y) + (x * surface->format->BytesPerPixel);
                    const float averageHeight = (float)(pixel[0] + pixel[1] + pixel[2]) / 3.0f;
                    height[x + y * w] = averageHeight * (HEIGHTMAP_MAX_HEIGHT / 255.0f);
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

            const GLuint v1 = index + 1;
            const GLuint v2 = index + 0;
            const GLuint v3 = index + w;
            const GLuint v4 = index + w + 1;

            indexArray.emplace_back(v1);
            indexArray.emplace_back(v2);
            indexArray.emplace_back(v3);

            indexArray.emplace_back(v1);
            indexArray.emplace_back(v3);
            indexArray.emplace_back(v4);
        }
    }

    mesh.setVertices(vertexArray);
    mesh.setIndices(indexArray);
}

/*
btBvhTriangleMeshShape *Heightmap::generateCollisionMesh()
{
    btScalar *collisionVertices = new btScalar[w * h * 3];
    unsigned short *collisionIndices = new unsigned short[w * h * 6];

    for(int z = 0; z < h; z++)
    {
        for(int x = 0; x < w; x++)
        {
            const size_t index = x + w * z;

            collisionVertices[(index * 3) + 0] = HEIGHTMAP_TILE_W * x;
            collisionVertices[(index * 3) + 1] = height[index];
            collisionVertices[(index * 3) + 2] = HEIGHTMAP_TILE_H * z;
        }
    }

    for(int z = 0; z < h - 1; z++)
    {
        for(int x = 0; x < w - 1; x++)
        {
            const size_t index = x + w * z;

            const size_t v1 = index + 1;
            const size_t v2 = index + 0;
            const size_t v3 = index + w;
            const size_t v4 = index + w + 1;

            collisionIndices[(index * 6) + 0] = v1;
            collisionIndices[(index * 6) + 1] = v2;
            collisionIndices[(index * 6) + 2] = v3;
            collisionIndices[(index * 6) + 3] = v1;
            collisionIndices[(index * 6) + 4] = v3;
            collisionIndices[(index * 6) + 5] = v4;
        }
    }
    btIndexedMesh part;
    part.m_vertexBase = (const unsigned char*)collisionVertices;
    part.m_vertexStride = sizeof(btScalar) * 3;
    part.m_numVertices = w * h;
    part.m_triangleIndexBase = (const unsigned char*)collisionIndices;
    part.m_triangleIndexStride = sizeof(unsigned short) * 3;
    part.m_numTriangles = w * h * 2;
    part.m_indexType = PHY_SHORT;

    // TODO: Free this, lol
    btTriangleIndexVertexArray* meshInterface = new btTriangleIndexVertexArray();
    meshInterface->addIndexedMesh(part, PHY_SHORT);

    return new btBvhTriangleMeshShape(meshInterface, true);
}
*/
