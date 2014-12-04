#version 400
uniform float minHeight;
uniform float maxHeight;
uniform float scale;
uniform mat4 modelViewProjectionMatrix;
layout(location = 0) in vec4 position;
layout(location = 10) in vec4 color;

in vec2 texcoord;
//out vec2 varyingtexcoord;
//out vec4 FS_in_vertexColor;

void main(){
    
	float lscale = 1.0;
    float offset = position.z - minHeight*lscale;
    float heightDelta = maxHeight - minHeight*lscale;
    float normalizedOffset = offset / heightDelta;
    float normalizedHeight = position.z / (maxHeight*lscale);
    

	//normalizedHeight = 1 - normalizedHeight;
//    FS_in_vertexColor = color;
//    FS_in_vertexColor = vec4(0,1,0,1);
//    if (position.z < 0) {
//        float normalizedHeightBlue = abs(position.z) / (maxHeight*lscale);
//        FS_in_vertexColor = vec4(0,0,1-normalizedHeightBlue,1);
//    }
//    else {
//        if (normalizedHeight > 0.8) {
//            FS_in_vertexColor = vec4(0.7*normalizedHeight,0.7*normalizedHeight,0.7*normalizedHeight,1);
//        }
//        else if (normalizedHeight > 0.35) {
//            FS_in_vertexColor = vec4(0.55*normalizedHeight,0.27*normalizedHeight,0.07*normalizedHeight,1);
//        }
//    }

    
    
    gl_Position = modelViewProjectionMatrix * position;
}