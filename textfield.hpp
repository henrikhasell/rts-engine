#ifndef TEXT_HPP
#define TEXT_HPP

#include "mesh2d.hpp"
#include "texture.hpp"
#include "font.hpp"

namespace Engine
{
namespace GL
{
class TextField
{
public:
    TextField();
    ~TextField();
    bool setText(Font &font, const char text[]);
    void draw(const Graphics &graphics) const;
    void draw(const Graphics &graphics, const glm::vec2 &position) const;
    float getW();
    float getH();
private:
    Texture texture;
    Mesh2D mesh;
    float w;
    float h;
};
};
};


#endif // TEXT_HPP
