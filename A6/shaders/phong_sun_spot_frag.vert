#version 150
in   vec4 vPosition;
in   vec3 vNormal;

// output values that will be interpretated per-fragment
out  vec3 fN;
out  vec3 fE;
out  vec3 directionalL;
out  vec3 flashL, flashLd;
out  vec4 p, p0;


uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform vec4 DirectionalLightPosition;
uniform vec4 FlashlightPosition;

void main()
{
    vec3 positionInCamera = (view_matrix*model_matrix*vPosition).xyz;
    fN = (view_matrix*model_matrix*vec4(vNormal, 0)).xyz;
    fE = -positionInCamera;
    directionalL = (view_matrix*DirectionalLightPosition).xyz;
    
    flashL = (view_matrix*FlashlightPosition).xyz-positionInCamera;
    flashLd = vec3(0,0,-1);

    p = view_matrix * FlashlightPosition;
    p0 = view_matrix * model_matrix * vPosition;

   gl_Position = proj_matrix * view_matrix * model_matrix * vPosition;
}
