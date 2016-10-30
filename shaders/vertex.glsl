#version 130

in vec4 in_Position;
in vec4 in_Normal;

out vec4 ex_Position;
out vec4 ex_Normal;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main(){
	ex_Position = modelMatrix * in_Position;
	ex_Normal = viewMatrix * modelMatrix *  in_Normal;

	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * in_Position;
}

