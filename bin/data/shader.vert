#version 400
uniform float minHeight;
uniform float maxHeight;
uniform float scale;

//Camera 
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;

//light 
uniform vec3 LightPosition ;

layout(location = 0) in vec4 position;
in vec3 normal;
layout(location = 10) in vec4 color;

in vec2 texcoord;

out vec4 GS_in_vertexColor;
//out vec4 FS_in_vertexColor;

void main(){
	//vec4 GS_in_vertexColor;

	float lscale = 1.0;
    float offset = position.z - minHeight*lscale;
    float heightDelta = maxHeight - minHeight*lscale;
    float normalizedOffset = offset / heightDelta;
    float normalizedHeight = position.z / (maxHeight*lscale);

	GS_in_vertexColor = vec4 (1. , 0. , 1. , 1.);


    vec4 position_camera_space = modelViewMatrix * position;

	vec3 normal_camera_space = normalize(( vec4(normal, 0.) * inverse(modelViewMatrix)).xyz);

	vec4 light_camera_space = modelViewMatrix * vec4(LightPosition, 1) ;

	vec3 light_direction = normalize(light_camera_space.xyz - position_camera_space.xyz);
	float light_intensity = dot(light_direction, normal_camera_space);
	
	GS_in_vertexColor = vec4(0,1,0,1);
    if (position.z < 0) {
        float normalizedHeightBlue = abs(position.z) / (maxHeight*lscale);
        GS_in_vertexColor = vec4(0,0,1-normalizedHeightBlue,1);
    }
    else {
        if (normalizedHeight > 0.8) {
            GS_in_vertexColor = vec4(0.7*normalizedHeight,0.7*normalizedHeight,0.7*normalizedHeight,1);
        }
        else if (normalizedHeight > 0.35) {
            GS_in_vertexColor = vec4(0.55*normalizedHeight,0.27*normalizedHeight,0.07*normalizedHeight,1);
        }
    }
	

	GS_in_vertexColor = vec4 ((GS_in_vertexColor * light_intensity).xyz , 1. ) ;

	//FS_in_vertexColor = GS_in_vertexColor;
    gl_Position = modelViewProjectionMatrix * position;
}