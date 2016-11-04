#include "camera.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;
using namespace GL;

Camera::Camera()
{
    //ctor
}

Camera::~Camera()
{
    //dtor
}

void Camera::setView(const Graphics &graphics)
{
    glm::mat4x4 view = glm::lookAt(
        glm::vec3( 500.0f, 500.0f, 500.0f ),
        glm::vec3( 500.0f,   0.0f,   0.0f ),
        glm::vec3(   0.0f,   1.0f,   0.0f )
    );

    view = glm::translate(view, -position);

    glUniformMatrix4fv(graphics.uniformV3D, 1, GL_FALSE, &view[0][0]);
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
