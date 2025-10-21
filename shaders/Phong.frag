#version 400 core
in vec3 worldPosition;
in vec3 worldNormal;
out vec4 frag_color;
uniform vec3 viewPos;
uniform int lightCount;

struct Light{
    vec3 position;
    vec3 diff;
    vec3 spec;
};
uniform Light lights[4];


void main() {
    vec3 objectColor = vec3(0.385, 0.647, 0.812);
    vec3 norm = normalize(worldNormal);
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 ambient = vec3(0.1, 0.1, 0.1) * objectColor;
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir = normalize(lights[i].position - worldPosition);
        vec3 reflectDir = reflect(-lightDir, norm);
        float diff = max(dot(norm, lightDir), 0.0);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        diffuse += diff * lights[i].diff * objectColor;
        specular += spec * lights[i].spec;
    }

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);
}