#include "button.hpp"
#include <iostream>

using namespace Engine;
using namespace GL;

Button::Button(std::vector<Texture> &apperance, float x, float y, float w, float h) :
    apperance(apperance), mesh(3 * 3), position(x, y), state(NORMAL), w(w), h(h)
{
    int edgeW = apperance[0].getW();
    int edgeH = apperance[0].getH();

    Mesh2D::createRectangle(mesh[0], 0.0f,      0.0f,      edgeW,         edgeH);
    Mesh2D::createRectangle(mesh[1], edgeW,     0.0f,      w - edgeW * 2, edgeH);
    Mesh2D::createRectangle(mesh[2], w - edgeW, 0.0f,      edgeW,         edgeH);
    Mesh2D::createRectangle(mesh[3], 0.0f,      edgeH,     edgeW,         h - edgeH * 2);
    Mesh2D::createRectangle(mesh[4], edgeW,     edgeH,     w - edgeW * 2, h - edgeH * 2);
    Mesh2D::createRectangle(mesh[5], w - edgeW, edgeH,     edgeW,         h - edgeH * 2);
    Mesh2D::createRectangle(mesh[6], 0.0f,      h - edgeH, edgeW,         edgeH);
    Mesh2D::createRectangle(mesh[7], edgeW,     h - edgeH, w - edgeW * 2, edgeH);
    Mesh2D::createRectangle(mesh[8], w - edgeW ,h - edgeH, edgeW,         edgeH);
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
        textField.draw(graphics, state == PRESSED ? textPosition + 2.0f : textPosition);
    }
}

void Button::setText(GL::Font &font, const char text[])
{
    textField.setText(font, text);
    textPosition.x = position.x + (w / 2.0f) - (textField.getW() / 2.0f);
    textPosition.y = position.y + (h / 2.0f) - (textField.getH() / 2.0f);
}
