#include "button.hpp"

using namespace Engine;
using namespace GL;

Button::Button(std::vector<Button> &apperance, float w, float h, float x, float y) :
    apperance(apperance),
    mesh(3 * 3)
{
    //ctor
}

Button::~Button()
{
    //dtor
}
