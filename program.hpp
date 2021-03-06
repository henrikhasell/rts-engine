#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <GL/glew.h>
#include "shader.hpp"

namespace Engine
{
namespace GL
{
class Program
{
friend class Graphics;
friend class Mesh3D;
friend class Mesh2D;

public:
    Program();
    ~Program();
    bool link(const Shader &a, const Shader &b);
    bool linkStatus();
    std::string infoLog();
private:
    GLuint program;
}; // Program
}; // GL
}; // Engine

#endif // PROGRAM_HPP
