// fragment shader

#version 400


in vec2 gs_texcoord;
in float gs_lightIntensity;

out vec4 outputColor;
uniform sampler2DRect tex0;

void main()
{
	outputColor = vec4 ((texture(tex0,gs_texcoord) * gs_lightIntensity).xyz , 1.f);
}