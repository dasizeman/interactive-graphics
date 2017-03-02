#version 150
// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 directionalL;
in  vec3 flashL, flashLd; 
in  vec3 fE;
in  vec3 p, p0;

out vec4 FragColor;

uniform vec4 DirectionalLightAmbient, DirectionalLightDiffuse, DirectionalLightSpecular;
uniform vec4 FlashlightAmbient, FlashlightDiffuse, FlashlightSpecular;
uniform vec4 AmbientMaterial, DiffuseMaterial, SpecularMaterial;

uniform float Shininess;

void main() 
{ 

    vec3 N = normalize(fN);
    vec3 E = normalize(fE);



    /*
    // ----- Directional Light -----
    vec3 L = normalize(directionalL);

    vec3 H = normalize( L + E );
    
    vec4 ambient = DirectionalLightAmbient * AmbientMaterial;

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DirectionalLightDiffuse * DiffuseMaterial;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*DirectionalLightSpecular * SpecularMaterial;

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 ) {
	specular = vec4(0.0, 0.0, 0.0, 1.0);
    }
    */

    // ----- Flashlight -----
    vec3 L = normalize(flashL);
    vec3 Ld = normalize(flashLd);

    float distanceFalloff = 1 / pow(length(p - p0), 2);

    vec3 H = normalize( L + E );
    
    vec4 ambient = DirectionalLightAmbient * AmbientMaterial * distanceFalloff;

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DirectionalLightDiffuse * DiffuseMaterial * distanceFalloff;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specularAdd = Ks*DirectionalLightSpecular * SpecularMaterial * distanceFalloff;

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 ) {
	specularAdd = vec4(0.0, 0.0, 0.0, 1.0);
    }

    vec4 specular = specularAdd;


    FragColor = ambient + diffuse + specular;
    FragColor.a = 1.0;
} 

