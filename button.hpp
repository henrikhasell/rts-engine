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
    Button(std::vector<Texture> &apperance, float w, float h, float x, float y);
    ~Button();
    void draw(const Graphics &graphics) const;
private:
    std::vector<Texture> &apperance;
    std::vector<Mesh2D> mesh;
}; // Button
}; // GUI
}; // Engine


#endif // BUTTON_HPP
