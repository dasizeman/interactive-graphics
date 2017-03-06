#version 150
// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 directionalL;
in  vec3 flashL, flashLd; 
in  vec3 fE;
in  vec4 p, p0;

out vec4 FragColor;

uniform vec4 DirectionalLightAmbient, DirectionalLightDiffuse, DirectionalLightSpecular;
uniform vec4 FlashlightAmbient, FlashlightDiffuse, FlashlightSpecular;
uniform vec4 AmbientMaterial, DiffuseMaterial, SpecularMaterial;

uniform float Shininess;

void main() 
{ 

    vec3 N = normalize(fN);
    vec3 E = normalize(fE);


    // ----- Directional Light -----
    vec3 L = normalize(directionalL);

    vec3 H = normalize( L + E );
    
    vec4 ambient = DirectionalLightAmbient * AmbientMaterial;

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DirectionalLightDiffuse * DiffuseMaterial;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*DirectionalLightSpecular * SpecularMaterial;

    // ----- Flashlight -----
    L = normalize(flashL);
    vec3 Ld = normalize(flashLd);

    float distanceFalloff = 1 / pow(length(p - p0), 2);
    float spotlightFactor = pow(max(dot(-L, Ld),0), 2);
    float theta = abs(acos(dot(-L,Ld)));
    if (theta > radians(15))
        spotlightFactor = 0;
    spotlightFactor *= distanceFalloff;

    H = normalize( L + E );
    
    ambient += DirectionalLightAmbient * AmbientMaterial * spotlightFactor;

    Kd = max(dot(L, N), 0.0);
    diffuse += Kd*DirectionalLightDiffuse * DiffuseMaterial * spotlightFactor;
    
    Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specularAdd = Ks*DirectionalLightSpecular * SpecularMaterial * spotlightFactor;

    specular += specularAdd;


    FragColor = ambient + diffuse + specular;
    FragColor.a = 1.0;
} 

