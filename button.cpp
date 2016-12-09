#include "button.hpp"

using namespace Engine;
using namespace GL;

Button::Button(std::vector<Texture> &apperance, float w, float h, float x, float y) :
    apperance(apperance),
    mesh(3 * 3)
{
    Mesh2D::createRectangle(mesh[0], 100, 100);
    Mesh2D::createRectangle(mesh[1], 100, 100);
    Mesh2D::createRectangle(mesh[2], 100, 100);
    Mesh2D::createRectangle(mesh[3], 100, 100);
    Mesh2D::createRectangle(mesh[4], 100, 100);
    Mesh2D::createRectangle(mesh[5], 100, 100);
}

Button::~Button()
{
    //dtor
}

void Button::draw(const Graphics &graphics) const
{
    glm::vec2 position(0.0f, 0.0f);

    for(size_t i = 0; i < mesh.size(); i++)
    {
        apperance[i].bind();
        mesh[i].draw(graphics, position);
        position += 100.0f;
    }
}
