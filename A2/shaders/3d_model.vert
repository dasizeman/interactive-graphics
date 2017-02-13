#version 150

in vec4 vPosition;
in vec4 vColorIn;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

flat out vec4 color;



void main() 
{ 
  gl_Position = model_matrix * vPosition;
  color = vColorIn;

}
