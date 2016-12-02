#include "mesh3d.hpp"
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

using namespace Engine;
using namespace GL;

#define VERTEX_BUFFER 0
#define NORMAL_BUFFER 1
#define INDEX_BUFFER 2

Mesh3D::Mesh3D() : numberOfIndices(0)
{
    glGenBuffers(3, &buffer[0]);
}

Mesh3D::~Mesh3D()
{
    glDeleteBuffers(3, &buffer[0]);
}

bool Mesh3D::load(const aiMesh *mesh)
{
    std::vector<glm::vec3> vertexArray;
    std::vector<glm::vec3> normalArray;
    std::vector<GLuint> indexArray;

    for(unsigned int j = 0; j < mesh->mNumFaces; j++)
    {
        aiFace *face = mesh->mFaces + j;

        for(unsigned int k = 0; k < face->mNumIndices; k++)
        {
            indexArray.emplace_back(face->mIndices[k]);
        }
    }

    for(unsigned int j = 0; j < mesh->mNumVertices; j++)
    {
        aiVector3D *vertex = mesh->mVertices + j;
        aiVector3D *vertexN = mesh->mNormals + j;

        const glm::vec3 position(vertex->x, vertex->y, vertex->z);
        const glm::vec3 normal(vertexN->x, vertexN->y, vertexN->z);

        vertexArray.push_back(position);
        normalArray.push_back(normal);
    }

    setVertices(vertexArray);
    setNormals(normalArray);
    setIndices(indexArray);

    return true;
}

void Mesh3D::setVertices(const std::vector<glm::vec3> &vertexArray)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(glm::vec3), vertexArray.data(), GL_STATIC_DRAW);
}

void Mesh3D::setNormals(const std::vector<glm::vec3> &normalArray)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer[NORMAL_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, normalArray.size() * sizeof(glm::vec3), normalArray.data(), GL_STATIC_DRAW);
}

void Mesh3D::setIndices(const std::vector<GLuint> &indexArray)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(GLuint), indexArray.data(), GL_STATIC_DRAW);
    numberOfIndices = indexArray.size();
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
    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, (void*)0);
}

void Mesh3D::draw(const Graphics &graphics, const glm::vec3 &position) const
{
    draw(graphics, position, glm::quat());
}

void Mesh3D::draw(const Graphics &graphics) const
{
    draw(graphics, glm::vec3(0.0, 0.0, 0.0));
}
/*
void Mesh3D::calculateNormals()
{
    std::vector<glm::vec3> normalArray;

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
*/
bool Model3D::loadFile(const char path[])
{
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path,
        aiProcess_Triangulate      |
        aiProcess_GenSmoothNormals |
        aiProcess_GenUVCoords      |
        aiProcess_SortByPType
        );

    if(scene)
    {
        return loadScene(scene);
    }

    return false;
}

bool Model3D::loadScene(const aiScene *scene)
{
    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        meshArray.emplace_back();
        meshArray.back().load(scene->mMeshes[i]);
    }
    return true;
}

void Model3D::draw(const Graphics &graphics) const
{
    for(const Mesh3D &mesh : meshArray)
    {
        mesh.draw(graphics);
    }
}
#undef VERTEX_BUFFER
#undef NORMAL_BUFFER
#undef INDEX_BUFFER
