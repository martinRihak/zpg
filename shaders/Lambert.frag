#version 400 core
in vec3 worldPosition;
in vec3 worldNormal;
out vec4 frag_color;
void main(){
    vec3 lightPosition = vec3(0.0, 0.0, 0.0);
    vec3 lightDirection = normalize(lightPosition - worldPosition);
    float diff = max(dot(normalize(lightDirection), normalize(worldNormal)), 0.0);

    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    vec4 objectColor = vec4(0.385, 0.647, 0.812, 1.0);
    frag_color = ambient + (diff * objectColor);
}