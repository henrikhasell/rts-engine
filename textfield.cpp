#include "textfield.hpp"

using namespace Engine;
using namespace GL;

TextField::TextField() : w(0.0f), h(0.0f)
{
    //ctor
}

TextField::~TextField()
{
    //dtor
}

bool TextField::setText(Font &font, const char text[])
{
    return font.renderString(mesh, texture, text, 100, w, h);
}

void TextField::draw(const Graphics &graphics) const
{
    texture.bind();
    mesh.draw(graphics);
}
void TextField::draw(const Graphics &graphics, const glm::vec2 &position) const
{
    texture.bind();
    mesh.draw(graphics, position);
}

float TextField::getW()
{
    return w;
}

float TextField::getH()
{
    return h;
}
