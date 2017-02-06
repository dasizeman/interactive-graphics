#version 150

in vec3 vPosition;

uniform mat3 model_view;

void main() 
{ 
    gl_Position = vec4((model_view*vPosition).xy,0,1);

}
