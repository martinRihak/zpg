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

void main() {
    vec3 norm = normalize(worldNormal);
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 ambient = vec3(0.1, 0.1, 0.1) * material.ambient ;
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    vec3 color;
    if(material.hasTexture == true)
        color = texture(material.ourTexture, texCoords).rgb;
    else
        color = material.objectColor;

    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir;
        float attenuation;

        if (lights[i].lightType == 2) {  
            lightDir = normalize(-lights[i].direction);  
            attenuation = 1.0; 
        } else {
            lightDir = normalize(lights[i].position - worldPosition); 
            float distance = length(lights[i].position - worldPosition);
            attenuation = 1.0 / (lights[i].constant + (lights[i].linear * distance) + lights[i].quadratic * (distance * distance));
        }

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
        
        vec3 currentDiffuse = diff * lights[i].diff * color * attenuation;
        vec3 currentSpecular = spec * lights[i].spec * attenuation;

        if (lights[i].lightType == 1) {  
            float theta = dot(lightDir, normalize(-lights[i].direction));
            if (theta > lights[i].outterCutOff) { 
                float epsilon = lights[i].cutOff - lights[i].outterCutOff;
                float intensity = clamp((theta - lights[i].outterCutOff) / epsilon, 0.0, 1.0);  
                
                currentDiffuse *= intensity;
                currentSpecular *= intensity;
            } else {
                currentDiffuse = vec3(0.0);
                currentSpecular = vec3(0.0);
            }
        }  

        diffuse += currentDiffuse  ;
        specular += currentSpecular * material.specular;
    }

    vec3 result =  ambient + diffuse + specular + material.emission;
    frag_color = vec4(result, 1.0);
}