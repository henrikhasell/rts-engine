#version 130

in vec4 in_Position;
// in vec4 in_Color;
// out vec4 ex_Color;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main(void)
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * in_Position;
	// ex_Color = in_Color;
}
