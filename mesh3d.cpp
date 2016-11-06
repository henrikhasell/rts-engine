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

void Mesh3D::draw(const Graphics &graphics, const glm::vec3 &position, const glm::quat &rotation) const
{
    glm::mat4x4 model = glm::toMat4(rotation);
    glm::mat4x4 view = glm::translate(graphics.matrixV3D, position);

    glUniformMatrix4fv(graphics.uniformM3D, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(graphics.uniformV3D, 1, GL_FALSE, &view[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glVertexAttribPointer(graphics.attributePosition3D, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[NORMAL_BUFFER]);
    glVertexAttribPointer(graphics.attributeNormal3D, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, (void*)0);
}

void Mesh3D::draw(const Graphics &graphics, const glm::vec3 &position) const
{
    draw(graphics, position, glm::quat());
}

void Mesh3D::draw(const Graphics &graphics) const
{
    draw(graphics, glm::vec3(0.0, 0.0, 0.0));
}

void Mesh3D::calculateNormals()
{
    normalArray.clear();
    normalArray.resize(vertexArray.size());

    for(size_t i = 0; i < indexArray.size(); i += 3)
    {
        const GLuint indexA = indexArray[i + 0];
        const GLuint indexB = indexArray[i + 1];
        const GLuint indexC = indexArray[i + 2];

        const glm::vec3 &t0 = vertexArray[indexA];
        const glm::vec3 &t1 = vertexArray[indexB];
        const glm::vec3 &t2 = vertexArray[indexC];

        const glm::vec3 v1 = t0 - t1;
        const glm::vec3 v2 = t0 - t2;

        const glm::vec3 &normal = glm::normalize(glm::cross(v1, v2));

        normalArray[indexA] += normal;
        normalArray[indexB] += normal;
        normalArray[indexC] += normal;
    }

    for(glm::vec3 &normal : normalArray)
    {
        normal = glm::normalize(normal);
    }

    setNormals(normalArray);
}

#undef VERTEX_BUFFER
#undef NORMAL_BUFFER
#undef INDEX_BUFFER
