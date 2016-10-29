#include "graphics.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;
using namespace GL;

Graphics::Graphics() :
    fragmentShader(GL_FRAGMENT_SHADER),
    vertexShader(GL_VERTEX_SHADER),
    program()
{
    // matrixP = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
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

                attributePosition = glGetAttribLocation(program.program, "position");
                //attributeNormal = glGetAttribLocation(program.program, "normal");
                attributeColour = glGetAttribLocation(program.program, "colour");

                // std::cout << "Position: " << attributePosition << std::endl;
                //std::cout << "Normal: " << attributeNormal << std::endl;
                // std::cout << "Colour: " << attributeColour << std::endl;

                glUniformMatrix4fv(uniformP, 1, GL_FALSE, &matrixP[0][0]);
                glUniformMatrix4fv(uniformV, 1, GL_FALSE, &matrixV[0][0]);
                glUniformMatrix4fv(uniformM, 1, GL_FALSE, &matrixM[0][0]);

                glEnable(GL_DEPTH_TEST);

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
