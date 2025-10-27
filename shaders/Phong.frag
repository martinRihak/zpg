#version 400 core
in vec3 worldPosition;
in vec3 worldNormal;
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
    int lightType;    // NOVÉ: 0 = point, 1 = spot, 2 = directional
};
uniform Light lights[8];  

void main() {
    vec3 objectColor = vec3(0.385, 0.647, 0.812);
    vec3 norm = normalize(worldNormal);
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 ambient = vec3(0.1, 0.1, 0.1) * objectColor;
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir;
        float attenuation;

        if (lights[i].lightType == 2) {  // Directional light – NOVÁ PODMÍNKA
            lightDir = normalize(-lights[i].direction);  // Směr je rovnoběžný, žádná pozice
            attenuation = 1.0;  // Žádný útlum
        } else {
            lightDir = normalize(lights[i].position - worldPosition);  // Pro point/spot
            float distance = length(lights[i].position - worldPosition);
            attenuation = 1.0 / (lights[i].constant + (lights[i].linear * distance) + lights[i].quadratic * (distance * distance));
        }

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
        
        // Přidání do diffuse a specular – stejné jako dříve, ale s attenuation
        vec3 currentDiffuse = diff * lights[i].diff * objectColor * attenuation;
        vec3 currentSpecular = spec * lights[i].spec * attenuation;

        if (lights[i].lightType == 1) {  
            float theta = dot(lightDir, normalize(-lights[i].direction));
            if (theta > lights[i].outterCutOff) { 
                float epsilon = lights[i].cutOff - lights[i].outterCutOff;
                float intensity = clamp((theta - lights[i].outterCutOff) / epsilon, 0.0, 1.0);  // OPRAVA: clamp a správné dělení
                
                currentDiffuse *= intensity;
                currentSpecular *= intensity;
            } else {
                currentDiffuse = vec3(0.0);
                currentSpecular = vec3(0.0);
            }
        }  

        diffuse += currentDiffuse;
        specular += currentSpecular;
    }

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);
}