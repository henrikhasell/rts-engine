#include "graphics.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;
using namespace GL;

Graphics::Graphics(SDL_Window *window) :
    fragmentShader3D(GL_FRAGMENT_SHADER),
    vertexShader3D(GL_VERTEX_SHADER),
    fragmentShader2D(GL_FRAGMENT_SHADER),
    vertexShader2D(GL_VERTEX_SHADER)
{
    int w;
    int h;

    SDL_GetWindowSize(window, &w, &h);

    GLfloat aspectRatio = (GLfloat)w/(GLfloat)h;

    matrixP3D = glm::perspective(45.0f, aspectRatio, 1.0f, 10000.0f);

    matrixP2D = glm::ortho(0.0f, (GLfloat)w, (GLfloat)h, 0.0f);

    matrixV3D = glm::lookAt(
        glm::vec3(1000.0f, 1000.0f, 1000.0f),
        glm::vec3(400.0f, 0.0f, 1000.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

bool Graphics::initialise()
{
    if(fragmentShader3D.load("shaders/fragment3D.glsl") == true)
    {
        if(vertexShader3D.load("shaders/vertex3D.glsl") == true)
        {
            if(program3D.link(fragmentShader3D, vertexShader3D) == true)
            {
                glUseProgram(program3D.program);

                attributePosition3D = glGetAttribLocation(program3D.program, "in_Position");
                attributeNormal3D = glGetAttribLocation(program3D.program, "in_Normal");
/*
                std::cout << "[3D] in_Position attribute location: " << attributePosition3D << std::endl;
                std::cout << "[3D] in_Normal attribute location: " << attributeNormal3D << std::endl;
*/
                uniformP3D = glGetUniformLocation(program3D.program, "projectionMatrix");
                uniformV3D = glGetUniformLocation(program3D.program, "viewMatrix");
                uniformM3D = glGetUniformLocation(program3D.program, "modelMatrix");
/*
                std::cout << "[3D] projectionMatrix: " << uniformP3D << std::endl;
                std::cout << "[3D] viewMatrix: " << uniformV3D << std::endl;
                std::cout << "[3D] modelMatrix: " << uniformM3D << std::endl;
*/
                glUniformMatrix4fv(uniformP3D, 1, GL_FALSE, &matrixP3D[0][0]);
                glUniformMatrix4fv(uniformV3D, 1, GL_FALSE, &matrixV3D[0][0]);
                glUniformMatrix4fv(uniformM3D, 1, GL_FALSE, &matrixM3D[0][0]);

                if(fragmentShader2D.load("shaders/fragment2D.glsl") == true)
                {
                    if(vertexShader2D.load("shaders/vertex2D.glsl") == true)
                    {
                        if(program2D.link(fragmentShader2D, vertexShader2D) == true)
                        {
                            glUseProgram(program2D.program);

                            attributePosition2D = glGetAttribLocation(program2D.program, "in_Position");
                            attributeUV2D = glGetAttribLocation(program2D.program, "in_UV");
/*
                            std::cout << "[2D] in_Position attribute location: " << attributePosition2D << std::endl;
                            std::cout << "[2D] in_UV attribute location: " << attributeUV2D << std::endl;
*/
                            uniformTextureSampler2D = glGetUniformLocation(program2D.program, "textureSampler");
                            uniformP2D = glGetUniformLocation(program2D.program, "projectionMatrix");
                            uniformV2D = glGetUniformLocation(program2D.program, "viewMatrix");
                            uniformM2D = glGetUniformLocation(program2D.program, "modelMatrix");

                            glUniform1i(uniformTextureSampler2D, 0);
                            glUniformMatrix4fv(uniformP2D, 1, GL_FALSE, &matrixP2D[0][0]);
                            glUniformMatrix4fv(uniformV2D, 1, GL_FALSE, &matrixV2D[0][0]);
                            glUniformMatrix4fv(uniformM2D, 1, GL_FALSE, &matrixM2D[0][0]);
/*
                            std::cout << "[2D] textureSampler: " << uniformTextureSampler2D << std::endl;
                            std::cout << "[2D] projectionMatrix: " << uniformP3D << std::endl;
                            std::cout << "[2D] viewMatrix: " << uniformV3D << std::endl;
                            std::cout << "[2D] modelMatrix: " << uniformM3D << std::endl;
*/
                            glEnable(GL_DEPTH_TEST);
                            //glEnable(GL_CULL_FACE);

                            glEnable(GL_BLEND);
                            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                            // glFrontFace(GL_CW);

                            return true;

                        }
                        else
                        {
                            std::cerr << "Failed to link program." << std::endl;
                        }
                    }
                    else
                    {
                        std::cerr << "Failed to compile vertex shader: " << vertexShader2D.compileLog() << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Failed to compile fragment shader: " << fragmentShader2D.compileLog() << std::endl;
                }
            }
            else
            {
                std::cerr << "Failed to link program." << std::endl;
            }
        }
        else
        {
            std::cerr << "Failed to compile vertex shader: " << vertexShader3D.compileLog() << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to compile fragment shader: " << fragmentShader3D.compileLog() << std::endl;
    }

    return false;
}

void Graphics::begin2D()
{
    glUseProgram(program2D.program);
    glEnableVertexAttribArray(attributePosition2D);
    glEnableVertexAttribArray(attributeUV2D);
}

void Graphics::begin3D()
{
    glUseProgram(program3D.program);
    glEnableVertexAttribArray(attributePosition3D);
    glEnableVertexAttribArray(attributeNormal3D);
}

void Graphics::end2D()
{
    glDisableVertexAttribArray(attributePosition2D);
    glDisableVertexAttribArray(attributeUV2D);
}

void Graphics::end3D()
{
    glDisableVertexAttribArray(attributePosition3D);
    glDisableVertexAttribArray(attributeNormal3D);

}

void Graphics::translate3D(const glm::vec3 &position)
{
    glm::mat4x4 model;
    model = glm::translate(model, position);
    glUniformMatrix4fv(uniformM3D, 1, GL_FALSE, &model[0][0]);
}
