#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <SDL2/SDL.h>
//#include <btBulletDynamicsCommon.h>
#include "graphics.hpp"
#include "mesh3d.hpp"

#define HEIGHTMAP_TILE_W 1
#define HEIGHTMAP_TILE_H 1
#define HEIGHTMAP_MAX_HEIGHT 10

namespace Engine
{
namespace GL
{
class Heightmap
{
public:
    Heightmap();
    ~Heightmap();
    void draw(const Graphics &graphics) const;
    bool load(const char path[]);
    void generateMesh();
    //btBvhTriangleMeshShape * generateCollisionMesh();
protected:
    Mesh3D mesh;
    float *height;

    int w;
    int h;
}; // Heightmap
}; // GL
}; // Engine

#endif // HEIGHTMAP_HPP
