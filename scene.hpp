#ifndef SCENE_HPP
#define SCENE_HPP

#include <assimp/scene.h>
#include "graphics.hpp"
#include "mesh.hpp"

namespace Engine
{
namespace GL
{
class Scene
{
public:
    Scene();
    ~Scene();
    bool load(const char path[]);
    void draw(const Graphics &graphics);
private:
    std::vector<Mesh> mesh;
};
};
};


#endif // SCENE_HPP
