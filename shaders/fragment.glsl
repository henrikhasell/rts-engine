#version 130

in vec4 ex_Position;
in vec4 ex_Normal;

out vec4 out_Color;

void main(void)
{
    vec3 lightDirection = vec3(-0.3, -0.4, -0.3);
    vec3 lightColour = vec3(1.0, 1.0, 1.0);
    float intensity = clamp(dot(lightDirection, ex_Normal.xyz), 0, 1);
	out_Color = vec4(lightColour * intensity, 1.0);
}
