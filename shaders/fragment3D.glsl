#version 130

in vec3 ex_Normal;
in vec2 ex_TexCoord;

out vec4 out_Color;

uniform sampler2D textureSampler;

void main(void)
{
    vec3 lightDirection = vec3(0.0, -1.0, 0.0);
    vec3 lightColour = vec3(0.2, 0.2, 0.2);
    vec3 ambientLight = vec3(0.5, 0.5, 0.5);
    float lightIntensity = dot(lightDirection, ex_Normal);
	out_Color = vec4(ambientLight + (lightColour * lightIntensity), 1.0);// * texture(textureSampler, ex_TexCoord);
}
