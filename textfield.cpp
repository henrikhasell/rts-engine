#include "textfield.hpp"

using namespace Engine;
using namespace GL;

TextField::TextField()
{
    //ctor
}

TextField::~TextField()
{
    //dtor
}

bool TextField::setText(const Font &font, const char text[])
{
    return font.renderString(mesh, texture, text);
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
