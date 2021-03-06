#include "shader.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace Engine;
using namespace GL;

Shader::Shader(GLenum type)
{
    shader = glCreateShader(type);
}

Shader::~Shader()
{
    glDeleteShader(shader);
}

bool Shader::load(const char path[])
{
    std::ifstream stream(path);

    if(stream.is_open() == true)
    {
        std::string source;

        while(stream.eof() != true)
        {
            std::string line;
            std::getline(stream, line);
            source += line + "\n";
        }

        return compile(source);
    }
    else
    {
        std::cerr << "Failed to open " << path << std::endl;
    }

    return false;
}

bool Shader::compile(std::string source)
{
    const GLchar *data = source.data();
    glShaderSource(shader, 1, &data, nullptr);
    glCompileShader(shader);
    return compileStatus();
}

bool Shader::compileStatus()
{
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    return result == GL_TRUE;
}

std::string Shader::infoLog()
{
    GLint length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    std::vector<char> buffer(length + 1);
    glGetShaderInfoLog(shader, length, NULL, buffer.data());
    return std::string(buffer.data());
}
