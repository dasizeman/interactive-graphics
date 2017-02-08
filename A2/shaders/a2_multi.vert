#version 130

in vec4 vPosition;
in vec4 vColorIn;

uniform mat3 model_view;
out vec4 color;



void main() 
{ 
  gl_Position = vPosition;
  color = vColorIn;

}