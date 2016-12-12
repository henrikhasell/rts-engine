#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <list>
#include <SDL2/SDL.h>
#include "button.hpp"

namespace Engine
{
class MouseHandler
{
    public:
        MouseHandler();
        ~MouseHandler();
        bool handleMouseMotionEvent(SDL_MouseMotionEvent *event);
        bool handleMouseButtonEvent(SDL_MouseButtonEvent *event);
        void addButton(GL::Button &button);
    protected:
        std::list<GL::Button*> buttonList;
        GL::Button *selected;
};
};

#endif // MOUSE_HPP
