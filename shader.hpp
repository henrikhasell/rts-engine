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
    bool load(const char path[]);
    bool compile(std::string source);
    bool compileStatus();
    std::string compileLog();
private:
    GLuint shader;
}; // Shader
}; // GL
}; // Engine

#endif // SHADER_HPP
