#version 130

in vec3 in_Position;
in vec2 in_TexCoord;

out vec2 ex_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main()
{
	ex_TexCoord = in_TexCoord;
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
}
