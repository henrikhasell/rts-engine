#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec3.hpp>
#include "graphics.hpp"

#define CAMERA_SPEED 10

namespace Engine
{
namespace GL
{
class Camera
{
public:
    Camera();
    ~Camera();
    void setView(const Graphics &graphics);
    void panForward();
    void panBackward();
    void panLeft();
    void panRight();
protected:
    glm::vec3 position;
    float height;
}; // Camera
}; // GL
}; // Engine

#endif // CAMERA_HPP
