#version 150
in   vec4 vPosition;
in   vec3 vNormal;

// output values that will be interpretated per-fragment
out  vec3 fN;
out  vec3 fE;
out  vec3 fL;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform vec4 LightPosition;

void main()
{
    fN = vNormal;
    fE = vPosition.xyz;
    fL = LightPosition.xyz;
    
    if( LightPosition.w != 0.0 ) {
	fL = LightPosition.xyz - vPosition.xyz;
    }

  gl_Position = proj_matrix * view_matrix * model_matrix * vPosition;
}
