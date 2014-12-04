#version 150

out vec3 N; // normal  
out vec3 v; // view  
void main(void)    
{       
   v = vec3(gl_ModelViewMatrix * gl_Vertex);         
   N = normalize(gl_NormalMatrix * gl_Normal);  
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;    
}  