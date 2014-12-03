// fragment shader
#version 400
 
// this is how we receive the texture
uniform sampler2DRect tex0;
in vec2 varyingtexcoord;
in vec4 c;
in float height;
out vec4 outputColor;
 
void main()
{
    //outputColor = texture(tex0,vec2(1,1));
//	outputColor = vec4(187/55,137/255,131/255,height);
	float r = 187.0/255.0;
	float g = 137.0/255.0;
	float b = 131.0/255.0;
	//outputColor = vec4(r,g,b,height);

	outputColor = c;
	//outputColor = vec4(1,1,1,1);
}