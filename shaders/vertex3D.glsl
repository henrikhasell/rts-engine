#version 130

in vec3 in_Position;
in vec3 in_ViewPosition;
in vec3 in_Normal;

out vec3 ex_Normal;
out vec3 ex_FragmentPosition;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main(){
    ex_FragmentPosition = vec3(modelMatrix * vec4(in_Position, 1.0f));
	ex_Normal = normalize((modelMatrix * vec4(in_Normal, 1.0)).xyz);
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
}
