#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>

namespace Engine
{
namespace GL
{
class Shader
{
friend class Program;

public:
    Shader(GLenum type);
    ~Shader();
    bool compile(std::string source);
    bool load(const char path[]);
    bool compileStatus();
private:
    GLuint shader;
}; // Shader
}; // GL
}; // Engine

#endif // SHADER_HPP
