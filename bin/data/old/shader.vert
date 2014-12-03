#version 400
 
// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform float minHeight;
uniform float maxHeight;
 
in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;
// this is the end of the default functionality
 
// this is something we're creating for this shader
out vec2 varyingtexcoord;
out vec4 c;
out float height;
 
// this is coming from our C++ code
uniform float mouseX;
 
void main()
{
    float offset = position.z - minHeight;
    float heightDelta = maxHeight - minHeight;
    float normalizedOffset = offset / heightDelta;

    // here we move the texture coordinates
    varyingtexcoord = vec2(texcoord.x + mouseX, texcoord.y);
 
    // send the vertices to the fragment shader
    gl_Position = modelViewProjectionMatrix * position;

	/*
	height = position.z;
	
	
	if (height <0) 
	{
	  height = 180 - abs(height);
	}
	else
	{
		height = height+180;
	}
	
	height = height/360;
	float r = 160.0/255.0;
	float g = 82.0/255.0;
	float b = 45.0/255.0;
	c = vec4(vec3(r*height,g*height,b*height),1);
	*/

    if (position.z < 0) {
        c = vec4(0,0,1,1);
    }
    else {
        float normalizedHeight = position.z / maxHeight;
        if (normalizedHeight > 0.8) {
            c = vec4(0.7,0.7,0.7,1);
        }
        else if (normalizedHeight > 0.35) {
            c = vec4(0.55,0.27,0.07,1);
        }
    }
	
}