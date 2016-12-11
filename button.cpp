#include "button.hpp"
#include <iostream>

using namespace Engine;
using namespace GL;

Button::Button(std::vector<Texture> &apperance, float x, float y, float w, float h) :
    apperance(apperance), mesh(3 * 3), state(NORMAL), position(x, y)
{
    int edgeW = apperance[0].getW();
    int edgeH = apperance[0].getH();

    Mesh2D::createRectangle(mesh[0], 0.0f,      0.0f,         edgeW,           edgeH);
    Mesh2D::createRectangle(mesh[1], edgeW,     0.0f,        w - edgeW * 2, edgeH);
    Mesh2D::createRectangle(mesh[2], w - edgeW, 0.0f,         edgeW,           edgeH);
    Mesh2D::createRectangle(mesh[3], 0.0f,      edgeH,           edgeW,           h - edgeH * 2);
    Mesh2D::createRectangle(mesh[4], edgeW,      edgeH,           w - edgeW * 2,           h - edgeH * 2);
    Mesh2D::createRectangle(mesh[5], w - edgeW,      edgeH,           edgeW,           h - edgeH * 2);
    Mesh2D::createRectangle(mesh[6], 0.0f ,h - edgeH,        edgeW, edgeH);
    Mesh2D::createRectangle(mesh[7], edgeW,     h - edgeH,        w - edgeW * 2, edgeH);
    Mesh2D::createRectangle(mesh[8], w - edgeW ,h - edgeH,        edgeW, edgeH);

    /*
    for(size_t i = 1; i < mesh.size(); i++)
    {
        Mesh2D::createRectangle(mesh[i], 25.0f, 25.0f);
    }
    */
}

Button::~Button()
{
    //dtor
}

void Button::buildMesh(const std::vector<Texture> &apperance)
{

}

void Button::draw(const Graphics &graphics) const
{
    size_t index;

    switch(state)
    {
        case NORMAL:
            index = 0; break;
        case HOVER:
            index = 18; break;
        case PRESSED:
            index = 9; break;
        case DISABLED:
            index = 27; break;
    }
    for(size_t i = 0; i < mesh.size(); i++)
    {
        apperance[index + i].bind();
        mesh[i].draw(graphics, position);
    }
}
