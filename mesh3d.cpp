#include "mesh3d.hpp"

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

void Mesh3D::draw(const Graphics &graphics)
{
    glUseProgram(graphics.program3D.program);

    glEnableVertexAttribArray(graphics.attributePosition3D);
    glEnableVertexAttribArray(graphics.attributeNormal3D);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glVertexAttribPointer(graphics.attributePosition3D, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[NORMAL_BUFFER]);
    glVertexAttribPointer(graphics.attributeNormal3D, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(graphics.attributePosition3D);
    glDisableVertexAttribArray(graphics.attributeNormal3D);
}

#undef VERTEX_BUFFER
#undef NORMAL_BUFFER
#undef INDEX_BUFFER
