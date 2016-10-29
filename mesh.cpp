#include "mesh.hpp"

using namespace Engine;
using namespace GL;

Mesh::Mesh()
{
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &colourBuffer);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &colourBuffer);
}

void Mesh::setVertices(const std::vector<glm::vec4> &vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &vertices, GL_STATIC_DRAW);
    this->vertex = vertices;
}

void Mesh::setColours(const std::vector<glm::vec4> &colours)
{
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(glm::vec4), &colours, GL_STATIC_DRAW);
    this->colour = colour;
}

void Mesh::draw(const Graphics &graphics)
{
    glEnableVertexAttribArray(graphics.attributePosition);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(graphics.attributePosition, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, vertex.size());
    glDisableVertexAttribArray(graphics.attributePosition);
}
