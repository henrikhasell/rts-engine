#version 130

in vec3 in_Position;
in vec3 in_Normal;

out vec4 ex_Position;
out vec4 ex_Normal;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main(){
	ex_Position = modelMatrix * vec4(in_Position, 1.0);
	ex_Normal = modelMatrix * vec4(in_Normal, 1.0);

	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
}

