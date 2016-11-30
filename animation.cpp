#include "animation.hpp"

using namespace Engine;
using namespace GL;

AnimatedMesh::AnimatedMesh()
{
    glGenBuffers(6, &buffer[0]);
}

AnimatedMesh::~AnimatedMesh()
{
    glDeleteBuffers(6, &buffer[0]);
}
