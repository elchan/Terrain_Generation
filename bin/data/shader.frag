// fragment shader

#version 400

in vec4 FS_in_vertexColor;
in vec2 gs_texcoord;
out vec4 outputColor;
uniform sampler2DRect tex0;

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
    
//    float windowWidth = 1024.0;
//    float windowHeight = 768.0;
//    
//    float r = gl_FragCoord.x / windowWidth;
//    float g = gl_FragCoord.y / windowHeight;
//    float b = 1.0;
//    float a = 1.0;
//    outputColor = vec4(r, g, b, a);
//	outputColor = vec4(0.5,0.5,0.5,1);
   // outputColor = FS_in_vertexColor;
	
	outputColor = texture(tex0,gs_texcoord);
	//outputColor = vec4(gs_texcoord, 0, 1);
}