#version 130

in vec4 ex_Position;
in vec4 ex_Normal;

out vec4 out_Color;

void main(void)
{
	out_Color = ex_Normal;
}
