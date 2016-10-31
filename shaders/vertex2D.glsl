#version 130

in vec3 in_Position;
in vec2 in_UV;

out vec2 ex_UV;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main()
{
	ex_UV = in_UV;
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
}

