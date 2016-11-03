#version 130

in vec2 ex_UV;

out vec4 out_Color;

uniform sampler2D textureSampler;

void main()
{
	out_Color = vec4(1.0, 0.0, 0.0, 1.0) * texture(textureSampler, ex_UV);
}
