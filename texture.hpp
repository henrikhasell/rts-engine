#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <vector>

namespace Engine
{
namespace GL
{
class Texture
{
    public:
        static void loadSpriteSheet(std::vector<Texture> &apperance, const char path[]);
        Texture();
        ~Texture();
        bool load(const char path[]);
        bool load(SDL_Surface *surface);
        void bind() const;
    private:
        GLuint texture;
}; // Texture
}; // GL
}; // Engine


#endif // TEXTURE_HPP
