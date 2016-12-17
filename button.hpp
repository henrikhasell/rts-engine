#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "texture.hpp"
#include "textfield.hpp"
#include "mesh2d.hpp"

namespace Engine
{
class MouseHandler;

namespace GL
{
class Button
{
friend MouseHandler;

public:
    enum State
    {
        NORMAL, HOVER, PRESSED, DISABLED
    };
    Button(std::vector<Texture> &apperance, float x, float y, float w, float h);
    ~Button();
    void draw(const Graphics &graphics) const;
    void setText(GL::Font &font, int size, const char text[]);
    void setCallback(void (*callback)(void));
private:
    void buildMesh(const std::vector<Texture> &apperance);
    GL::TextField textField;
    std::vector<Texture> &apperance;
    std::vector<Mesh2D> mesh;
    glm::vec2 position;
    glm::vec2 textPosition;
    State state;
    void (*callback)(void);
    float w;
    float h;
}; // Button
}; // GUI
}; // Engine


#endif // BUTTON_HPP
