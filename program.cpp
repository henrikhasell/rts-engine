#include "program.hpp"
#include <vector>

using namespace Engine;
using namespace GL;

Program::Program()
{
    program = glCreateProgram();
}

Program::~Program()
{
    glDeleteProgram(program);
}

bool Program::link(const Shader &a, const Shader &b)
{
    glAttachShader(program, a.shader);
    glAttachShader(program, b.shader);
    glLinkProgram(program);
    return linkStatus();
}

bool Program::linkStatus()
{
    GLint result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    return result == GL_TRUE;
}

std::string Program::infoLog()
{
    GLint length;
	glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);

    std::vector<char> buffer(length + 1);
    glGetProgramInfoLog(program, length, NULL, buffer.data());
    return std::string(buffer.data());
}
