#version 130

in vec4 ex_Position;
in vec4 ex_Normal;

out vec4 out_Color;

void main(void)
{
    vec3 lightDirection = vec3(0.3, 0.6, 0.1);
    vec3 lightColour = vec3(1.0, 1.0, 1.0);
	out_Color = vec4(lightColour * dot(lightDirection, ex_Normal.xyz), 1.0);
}
