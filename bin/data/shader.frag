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
<<<<<<< HEAD
	if(height< 0.0 ) 
	{
		outputColor = vec4 ((texture(tex1,gs_texcoord) * gs_lightIntensity).xyz , 1.);
	}
	else 
		outputColor = vec4 ((texture(tex0,gs_texcoord) * gs_lightIntensity).xyz , 1.);
=======
	outputColor = vec4 ((texture(tex0,gs_texcoord) * gs_lightIntensity).xyz , 1.);
>>>>>>> 645f7668009d94be2c9d0f82f161366be109bb00
}