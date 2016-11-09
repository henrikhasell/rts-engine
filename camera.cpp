#include "camera.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;
using namespace GL;

Camera::Camera() : position(0.0f, 0.0f, 0.0f)
{
    //ctor
}

Camera::~Camera()
{
    //dtor
}

void Camera::applyView(Graphics &graphics)
{
    glm::mat4x4 view = glm::lookAt(
        glm::vec3(
            position.x + 0.f,
            position.y + 100.0f,
            position.z + 100.0f
        ),// 50, 30
        position,
        glm::vec3( 0.0f, 1.0f, 0.0f )
    );

    glUniformMatrix4fv(graphics.uniformV3D, 1, GL_FALSE, &view[0][0]);
    graphics.matrixV3D = view;

    glUniform3f(graphics.uniformViewPosition3D,
        position.x,
        position.y + 100.0f,
        position.z + 100.0f
    );
}

void Camera::panForward()
{
    position.z -= CAMERA_SPEED;
}

void Camera::panBackward()
{
    position.z += CAMERA_SPEED;
}

void Camera::panLeft()
{
    position.x -= CAMERA_SPEED;
}

void Camera::panRight()
{
    position.x += CAMERA_SPEED;
}
