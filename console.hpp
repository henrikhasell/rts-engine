#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <list>
#include <lua5.2/lua.hpp>
#include "textfield.hpp"
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
    void submitInput(const Font &font, lua_State *luaState);
    void backspace(const Font &font);
    void draw(const Graphics &graphics);
protected:
    std::list<TextField> outputText;
    TextField inputText;
    std::string inputString;
}; // Console
}; // GL
}; // Engine

#endif // CONSOLE_HPP
