#include "mouse.hpp"

using namespace Engine;

MouseHandler::MouseHandler() : selected(nullptr)
{
    //ctor
}

MouseHandler::~MouseHandler()
{
    //dtor
}

void MouseHandler::addButton(GL::Button &button)
{
    buttonList.push_back(&button);
}

bool MouseHandler::handleMouseMotionEvent(SDL_MouseMotionEvent *event)
{
    if(selected)
    {
        if(event->x >= selected->position.x && event->x <= selected->position.x + selected->w)
        {
            if(event->y >= selected->position.y && event->y <= selected->position.y + selected->h)
            {
                selected->state = GL::Button::State::PRESSED;
                return true;
            }
        }
        selected->state = GL::Button::State::NORMAL;
    }
    else
    {
        for(GL::Button *button : buttonList)
        {
            if(event->x >= button->position.x && event->x <= button->position.x + button->w)
            {
                if(event->y >= button->position.y && event->y <= button->position.y + button->h)
                {
                    button->state = GL::Button::State::HOVER;
                    break;
                }
            }

            button->state = GL::Button::State::NORMAL;
        }
    }

    return false;
}
bool MouseHandler::handleMouseButtonEvent(SDL_MouseButtonEvent *event)
{
    if(event->state == SDL_RELEASED)
    {
        if(selected)
        {
            if(
            event->x >= selected->position.x && event->x <= selected->position.x + selected->w &&
            event->y >= selected->position.y && event->y <= selected->position.y + selected->h
            )
            {
                if(selected->callback) {
                    selected->callback();
                }
                selected->state = GL::Button::State::HOVER;
            }
            else
            {
                selected->state = GL::Button::State::NORMAL;
            }
            selected = nullptr;
        }
    }
    else if(event->state == SDL_PRESSED)
    {
        for(GL::Button *button : buttonList)
        {
            if(event->x >= button->position.x && event->x <= button->position.x + button->w)
            {
                if(event->y >= button->position.y && event->y <= button->position.y + button->h)
                {
                    button->state = GL::Button::State::PRESSED;
                    selected = button;
                    break;
                }
            }
        }
    }
    return false;
}
