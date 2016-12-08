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
    bool setText(const Font &font, const char text[]);
    void draw(const Graphics &graphics) const;
    void draw(const Graphics &graphics, const glm::vec2 &position) const;
private:
    Texture texture;
    Mesh2D mesh;
};
};
};


#endif // TEXT_HPP
