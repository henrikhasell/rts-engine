#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "shader.hpp"
#include "program.hpp"

#include <SDL2/SDL.h>
#include <glm/mat4x4.hpp>

namespace Engine
{
namespace GL
{
class Graphics
{
friend class Mesh3D;
friend class Mesh2D;
friend class AnimatedMesh;
friend class AnimatedModel;
friend class Texture;
friend class Camera;

public:
    Graphics(SDL_Window *window);
    bool initialise();

    void begin2D();
    void begin3D();
    void beginAnim();

    void end2D();
    void end3D();
    void endAnim();

    void translate3D(const glm::vec3 &position);
protected:
    bool initialise2D();
    bool initialise3D();
    bool initialiseAnim();

    void getAttribs2D();
    void getAttribs3D();
    void getAttribsAnim();

    void getUniforms2D();
    void getUniforms3D();
    void getUniformsAnim();
private:
    // Fragment shaders:
    Shader fragmentShader3D;
    Shader fragmentShader2D;
    Shader fragmentShaderAnim;
    // Vertex shaders:
    Shader vertexShader3D;
    Shader vertexShader2D;
    Shader vertexShaderAnim;
    // Programs:
    Program program3D;
    Program program2D;
    Program programAnim;
    // 3D Matrices:
    glm::mat4x4 matrixP3D;
    glm::mat4x4 matrixV3D;
    glm::mat4x4 matrixM3D;
    // 2D Matrices:
    glm::mat4x4 matrixP2D;
    glm::mat4x4 matrixV2D;
    glm::mat4x4 matrixM2D;
    // Anim Matrices:
    glm::mat4x4 matrixPAnim;
    glm::mat4x4 matrixVAnim;
    glm::mat4x4 matrixMAnim;
    // 3D Uniforms:
    GLint uniformP3D;
    GLint uniformV3D;
    GLint uniformM3D;
    // 2D Uniforms:
    GLint uniformP2D;
    GLint uniformV2D;
    GLint uniformM2D;
    // Anim Uniforms:
    GLint uniformPAnim;
    GLint uniformVAnim;
    GLint uniformMAnim;
    GLint uniformBoneMatricesAnim;
    // 3D Attributes:
    GLint attributePosition3D;
    GLint attributeNormal3D;
    // 2D Attributes:
    GLint attributePosition2D;
    GLint attributeTexCoord2D;
    // Anim Attributes:
    GLint attributePositionAnim;
    GLint attributeNormalAnim;
    GLint attributeTexCoordAnim;
    GLint attributeBoneWeightsAnim;
    GLint attributeBoneIndicesAnim;
}; // Graphics
}; // GL
}; // Engine


#endif // GRAPHICS_HPP
