#version 450 core
in vec3 worldPosition;
in vec3 worldNormal;
in vec2 texCoords;
out vec4 frag_color;
uniform vec3 viewPos;
uniform int lightCount;


struct Light {
    vec3 position;
    vec3 diff;
    vec3 spec;
    float constant;   
    float linear;     
    float quadratic;  
    vec3 direction;
    float cutOff;
    float outterCutOff;
    int lightType;    // 0 = point, 1 = spot, 2 = directional
};
uniform Light lights[8];  

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 objectColor;
    float shininess;
    vec3 emission;
    bool hasTexture;
    sampler2D ourTexture;
};
uniform Material material;

void main(){
    vec4 objectColor = vec4(material.objectColor, 1.0);
    frag_color =  objectColor;}

