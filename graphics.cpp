#include "graphics.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;
using namespace GL;

Graphics::Graphics(SDL_Window *window) :
    fragmentShader3D(GL_FRAGMENT_SHADER),
    fragmentShader2D(GL_FRAGMENT_SHADER),
    vertexShader3D(GL_VERTEX_SHADER),
    vertexShader2D(GL_VERTEX_SHADER)
{
    int w;
    int h;

    SDL_GetWindowSize(window, &w, &h);

    GLfloat aspectRatio = (GLfloat)w/(GLfloat)h;

    matrixP2D = glm::ortho(0.0f, (GLfloat)w, (GLfloat)h, 0.0f);

    matrixP3D = glm::perspective(45.0f, aspectRatio, 1.0f, 10000.0f);

    matrixV3D =  glm::lookAt(
        glm::vec3(
            10.f,
            5.0f,
            -10.0f
        ),
        glm::vec3( 0.0f, 5.0f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f )
    );
}

bool Graphics::initialise()
{
    bool result = initialise2D() && initialise3D();

    if(result)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        //glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
        //glFrontFace(GL_CW);
    }

    return result;
}

bool Graphics::initialise2D()
{
    bool result = false;

    if(fragmentShader2D.load("shaders/fragment2D.glsl") == true)
    {
        if(vertexShader2D.load("shaders/vertex2D.glsl") == true)
        {
            if(program2D.link(fragmentShader2D, vertexShader2D) == true)
            {
                glUseProgram(program2D.program);
                getAttribs2D();
                getUniforms2D();
                result = true;
            }
            else
            {
                std::cerr << "Failed to link program: " << program2D.infoLog() << std::endl;
            }
        }
        else
        {
            std::cerr << "Failed to compile vertex shader: " << vertexShader2D.infoLog() << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to compile fragment shader: " << fragmentShader2D.infoLog() << std::endl;
    }

    return result;
}

bool Graphics::initialise3D()
{
    bool result = false;

    if(fragmentShader3D.load("shaders/fragment3D.glsl") == true)
    {
        if(vertexShader3D.load("shaders/vertex3D.glsl") == true)
        {
            if(program3D.link(fragmentShader3D, vertexShader3D) == true)
            {
                glUseProgram(program3D.program);
                getAttribs3D();
                getUniforms3D();
                result = true;
            }
            else
            {
                std::cerr << "Failed to link program: " << program3D.infoLog() << std::endl;
            }
        }
        else
        {
            std::cerr << "Failed to compile vertex shader: " << vertexShader3D.infoLog() << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to compile fragment shader: " << fragmentShader3D.infoLog() << std::endl;
    }

    return result;
}

void Graphics::getAttribs2D()
{
    attributePosition2D = glGetAttribLocation(program2D.program, "in_Position");
    attributeTexCoord2D = glGetAttribLocation(program2D.program, "in_TexCoord");
}

void Graphics::getAttribs3D()
{
    attributePosition3D = glGetAttribLocation(program3D.program, "in_Position");
    attributeNormal3D = glGetAttribLocation(program3D.program, "in_Normal");
    attributeTexCoord3D = glGetAttribLocation(program3D.program, "in_TexCoord");
    attributeBoneWeights3D = glGetAttribLocation(program3D.program, "in_BoneWeights");
    attributeBoneIndices3D = glGetAttribLocation(program3D.program, "in_BoneIndices");
}

void Graphics::getUniforms2D()
{
    uniformP2D = glGetUniformLocation(program2D.program, "projectionMatrix");
    uniformV2D = glGetUniformLocation(program2D.program, "viewMatrix");
    uniformM2D = glGetUniformLocation(program2D.program, "modelMatrix");
}

void Graphics::getUniforms3D()
{
    uniformP3D = glGetUniformLocation(program3D.program, "projectionMatrix");
    uniformV3D = glGetUniformLocation(program3D.program, "viewMatrix");
    uniformM3D = glGetUniformLocation(program3D.program, "modelMatrix");
    uniformBoneMatrices3D = glGetUniformLocation(program3D.program, "boneMatrices");
}

void Graphics::begin2D()
{
    glUseProgram(program2D.program);
    glEnableVertexAttribArray(attributePosition2D);
    glEnableVertexAttribArray(attributeTexCoord2D);

    glUniformMatrix4fv(uniformP2D, 1, GL_FALSE, &matrixP2D[0][0]);
    glUniformMatrix4fv(uniformV2D, 1, GL_FALSE, &matrixV2D[0][0]);
    glUniformMatrix4fv(uniformM2D, 1, GL_FALSE, &matrixM2D[0][0]);
}

void Graphics::begin3D()
{
    glUseProgram(program3D.program);

    glEnableVertexAttribArray(attributePosition3D);
    glEnableVertexAttribArray(attributeNormal3D);
    glEnableVertexAttribArray(attributeTexCoord3D);
    glEnableVertexAttribArray(attributeBoneWeights3D);
    glEnableVertexAttribArray(attributeBoneIndices3D);

    glUniformMatrix4fv(uniformP3D, 1, GL_FALSE, &matrixP3D[0][0]);
    glUniformMatrix4fv(uniformV3D, 1, GL_FALSE, &matrixV3D[0][0]);
    glUniformMatrix4fv(uniformM3D, 1, GL_FALSE, &matrixM3D[0][0]);
}

void Graphics::end2D()
{
    glDisableVertexAttribArray(attributePosition2D);
    glDisableVertexAttribArray(attributeTexCoord2D);
    glDisableVertexAttribArray(attributeTexCoord2D);
}

void Graphics::end3D()
{

    glDisableVertexAttribArray(attributePosition3D);
    glDisableVertexAttribArray(attributeNormal3D);
    glDisableVertexAttribArray(attributeTexCoord3D);
    glDisableVertexAttribArray(attributeBoneWeights3D);
    glDisableVertexAttribArray(attributeBoneIndices3D);
}

void Graphics::translate3D(const glm::vec3 &position)
{
    glm::mat4x4 model;
    model = glm::translate(model, position);
    glUniformMatrix4fv(uniformM3D, 1, GL_FALSE, &model[0][0]);
}
