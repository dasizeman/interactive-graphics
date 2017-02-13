#version 150

in vec4 vPosition;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;



void main() 
{ 
  gl_Position = model_matrix*vPosition;

}
