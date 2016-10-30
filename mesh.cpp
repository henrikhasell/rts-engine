#include "mesh.hpp"
#include <iostream>

using namespace Engine;
using namespace GL;

#define VERTEX_BUFFER 1
#define COLOUR_BUFFER 2
#define INDEX_BUFFER 3

Mesh::Mesh()
{
    glGenBuffers(3, &buffer[0]);
}

Mesh::~Mesh()
{
    glDeleteBuffers(3, &buffer[0]);
}

void Mesh::setVertices(const std::vector<glm::vec4> &vertexArray)
{
    this->vertexArray = vertexArray;
    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(glm::vec4), vertexArray.data(), GL_STATIC_DRAW);
}
/*
void Mesh::setColours(const std::vector<glm::vec4> &colourArray)
{
    this->colourArray = colourArray;
    glBindBuffer(GL_ARRAY_BUFFER, buffer[COLOUR_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, colourArray.size() * sizeof(glm::vec4), colourArray.data(), GL_STATIC_DRAW);
}
*/
void Mesh::setIndices(const std::vector<GLuint> &indexArray)
{
    this->indexArray = indexArray;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(GLuint), indexArray.data(), GL_STATIC_DRAW);
}

void Mesh::draw(const Graphics &graphics)
{
    glEnableVertexAttribArray(graphics.attributePosition);
    // glEnableVertexAttribArray(graphics.attributeColour);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glVertexAttribPointer(graphics.attributePosition, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
/*
    glBindBuffer(GL_ARRAY_BUFFER, buffer[COLOUR_BUFFER]);
    glVertexAttribPointer(graphics.attributeColour, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(graphics.attributePosition);
    // glDisableVertexAttribArray(graphics.attributeColour);
}

#undef VERTEX_BUFFER
#undef COLOUR_BUFFER
#undef INDEX_BUFFER
