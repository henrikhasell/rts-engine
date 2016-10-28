#include "graphics.hpp"

#include <iostream>

using namespace Engine;
using namespace GL;

Graphics::Graphics() :
    fragmentShader(GL_FRAGMENT_SHADER),
    vertexShader(GL_VERTEX_SHADER),
    program()
{
    //ctor
}

bool Graphics::initialise()
{
    if(fragmentShader.load("shaders/fragment.glsl") == true)
    {
        if(vertexShader.load("shaders/vertex.glsl") == true)
        {
            if(program.link(fragmentShader, vertexShader) == true)
            {
                glUseProgram(program.program);

                uniformP = glGetUniformLocation(program.program, "projectionMatrix");
                uniformV = glGetUniformLocation(program.program, "viewMatrix");
                uniformM = glGetUniformLocation(program.program, "modelMatrix");

                glUniformMatrix4fv(uniformP, 1, GL_FALSE, &matrixP[0][0]);
                glUniformMatrix4fv(uniformV, 1, GL_FALSE, &matrixV[0][0]);
                glUniformMatrix4fv(uniformM, 1, GL_FALSE, &matrixM[0][0]);

                return true;
            }
            else
            {
                std::cerr << "Failed to link program." << std::endl;
            }
        }
        else
        {
            std::cerr << "Failed to compile vertex shader." << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to compile fragment shader." << std::endl;
    }

    return false;
}
