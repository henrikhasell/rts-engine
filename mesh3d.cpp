#include "mesh3d.hpp"
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;
using namespace GL;

#define VERTEX_BUFFER 0
#define NORMAL_BUFFER 1
#define INDEX_BUFFER 2

Mesh3D::Mesh3D()
{
    glGenBuffers(3, &buffer[0]);
}

Mesh3D::~Mesh3D()
{
    glDeleteBuffers(3, &buffer[0]);
}

void Mesh3D::setVertices(const std::vector<glm::vec3> &vertexArray)
{
    this->vertexArray = vertexArray;
    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(glm::vec3), vertexArray.data(), GL_STATIC_DRAW);
}

void Mesh3D::setNormals(const std::vector<glm::vec3> &normalArray)
{
    this->normalArray = normalArray;
    glBindBuffer(GL_ARRAY_BUFFER, buffer[NORMAL_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, normalArray.size() * sizeof(glm::vec3), normalArray.data(), GL_STATIC_DRAW);
}

void Mesh3D::setIndices(const std::vector<GLuint> &indexArray)
{
    this->indexArray = indexArray;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(GLuint), indexArray.data(), GL_STATIC_DRAW);
}

void Mesh3D::draw(const Graphics &graphics, const glm::vec3 &position)
{
    glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0), position);
    glUniformMatrix4fv(graphics.uniformM3D, 1, GL_FALSE, &transform[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glVertexAttribPointer(graphics.attributePosition3D, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[NORMAL_BUFFER]);
    glVertexAttribPointer(graphics.attributeNormal3D, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, (void*)0);
}

void Mesh3D::draw(const Graphics &graphics)
{
    draw(graphics, glm::vec3(0.0, 0.0, 0.0));
}

void Mesh3D::calculateNormals()
{
    if(normalArray.size() > 0)
    {
        // Normals already exist.
        // Do not continue.
        return;
    }
}

#undef VERTEX_BUFFER
#undef NORMAL_BUFFER
#undef INDEX_BUFFER
