#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <list>
#include <lua5.2/lua.h>
#include "mesh2d.hpp"
#include "font.hpp"

namespace Engine
{
namespace GL
{
class Console
{
public:
    Console();
    ~Console();

    void appendOutput(const Font &font, const char output[]);
    void appendInput(const Font &font, const char input[]);
    void submitInput(const Font &font, lua_State *state);
    void backspace(const Font &font);
    void draw(const Graphics &graphics);
protected:
    std::list<Engine::GL::Mesh2D> outputMesh;
    Engine::GL::Mesh2D inputMesh;
    std::string inputString;
}; // Console
}; // GL
}; // Engine

#endif // CONSOLE_HPP
