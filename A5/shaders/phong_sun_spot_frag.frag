#version 150
// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 fL;
in  vec3 fE;

out vec4 FragColor;

uniform vec4 AmbientLight, DiffuseLight, SpecularLight;
uniform vec4 AmbientMaterial, DiffuseMaterial, SpecularMaterial;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform float Shininess;

void main() 
{ 
    // Normalize the input lighting vectors
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);

    vec3 H = normalize( L + E );
    
    vec4 ambient = AmbientLight * AmbientMaterial;

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DiffuseLight * DiffuseMaterial;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularLight * SpecularMaterial;

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 ) {
	specular = vec4(0.0, 0.0, 0.0, 1.0);
    }

    FragColor = ambient + diffuse + specular;
    FragColor.a = 1.0;
} 

