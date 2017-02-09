#version 150

in vec4 vPosition;

uniform mat4 model_view;

void main() 
{ 
    gl_Position = model_view*vPosition;

}
