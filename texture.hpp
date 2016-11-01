#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <SDL2/SDL.h>

namespace Engine
{
namespace GL
{
class Texture
{
    public:
        Texture();
        ~Texture();
        bool load(SDL_Surface *surface);
        void bind();
    private:
        GLuint texture;
}; // Texture
}; // GL
}; // Engine


#endif // TEXTURE_HPP
