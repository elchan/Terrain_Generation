// fragment shader

#version 400


in vec2 gs_texcoord;
in float gs_lightIntensity;
in float height;

out vec4 outputColor;
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

void main()
{

	if(height< 0.0 ) 
	{
		outputColor = vec4 ((texture(tex1,gs_texcoord) * gs_lightIntensity).xyz , 1.);
	}
	else 
		outputColor = vec4 ((texture(tex0,gs_texcoord) * gs_lightIntensity).xyz , 1.);
}