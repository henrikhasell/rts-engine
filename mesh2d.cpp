#include "mesh2d.hpp"
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define VERTEX_BUFFER 0
#define INDEX_BUFFER 1
#define UV_BUFFER 2

using namespace Engine;
using namespace GL;

Mesh2D::Mesh2D()
{
    glGenBuffers(3, &buffer[0]);
}

Mesh2D::~Mesh2D()
{
    glDeleteBuffers(3, &buffer[0]);
}

void Mesh2D::setVertices(const std::vector<glm::vec2> &vertexArray)
{
    this->vertexArray = vertexArray;
    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(glm::vec2), vertexArray.data(), GL_STATIC_DRAW);
}

void Mesh2D::setTextureCoordinates(const std::vector<glm::vec2> &uvArray)
{
    this->uvArray = uvArray;
    glBindBuffer(GL_ARRAY_BUFFER, buffer[UV_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, uvArray.size() * sizeof(glm::vec2), uvArray.data(), GL_STATIC_DRAW);
}

void Mesh2D::setIndices(const std::vector<GLuint> &indexArray)
{
    this->indexArray = indexArray;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(GLuint), indexArray.data(), GL_STATIC_DRAW);
}


void Mesh2D::draw(const Graphics &graphics, const glm::vec2 &position)
{
    glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0), glm::vec3(position, 0.0));
    glUniformMatrix4fv(graphics.uniformM2D, 1, GL_FALSE, &transform[0][0]);

    texture.bind();

    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glVertexAttribPointer(graphics.attributePosition3D, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[UV_BUFFER]);
    glVertexAttribPointer(graphics.attributeUV2D, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, (void*)0);
}

void Mesh2D::draw(const Graphics &graphics)
{
    draw(graphics, glm::vec2(0, 0));
}

void Mesh2D::setTexture(SDL_Surface *surface)
{
    texture.load(surface);
}

#undef VERTEX_BUFFER
#undef INDEX_BUFFER
#undef UV_BUFFER