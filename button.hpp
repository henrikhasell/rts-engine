#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "texture.hpp"
#include "mesh2d.hpp"

namespace Engine
{
namespace GL
{
class Button
{
public:
    enum State
    {
        NORMAL, HOVER, PRESSED, DISABLED
    };
    Button(std::vector<Texture> &apperance, float x, float y, float w, float h);
    ~Button();
    void draw(const Graphics &graphics) const;
private:
    void buildMesh(const std::vector<Texture> &apperance);
    glm::vec2 position;
    std::vector<Texture> &apperance;
    std::vector<Mesh2D> mesh;
    State state;
}; // Button
}; // GUI
}; // Engine


#endif // BUTTON_HPP
