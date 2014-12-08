#version 400
uniform float minHeight;
uniform float maxHeight;
uniform float scale;

//Camera 
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

//light 
uniform vec3 LightPosition ;

layout(location = 0) in vec4 position;
in vec3 normal;
in vec2 texcoord;
layout(location = 10) in vec4 color;


out vec2 vs_texcoord;
out float vs_lightIntensity;
out vec4 vs_vertexWorldSpace;
out float vs_height;


void main(){

	float lscale = 1.0;
    float offset = position.z - minHeight*lscale;
    float heightDelta = maxHeight - minHeight*lscale;
    float normalizedOffset = offset / heightDelta;
    float normalizedHeight = position.z / (maxHeight*lscale);



  vec4 position_camera_space = modelViewMatrix * position;

	vec3 normal_camera_space = normalize(( vec4(normal, 0.) * inverse(modelViewMatrix)).xyz);

	vec4 light_camera_space = modelViewMatrix * vec4(LightPosition, 1) ;

	vec3 light_direction = normalize(light_camera_space.xyz - position_camera_space.xyz);
	float light_intensity = dot(light_direction, normal_camera_space);
	vs_texcoord = texcoord;
	//vs_texcoord = (light_camera_space.xy+vec2(1,1))/2.0;
	vs_lightIntensity = light_intensity;
	vs_height = position.z;


  gl_Position = modelViewProjectionMatrix * position;
  //gl_Position = camera1ProjectionMatrix * camera1ModelViewMatrix * position;
  vs_vertexWorldSpace = position;

}