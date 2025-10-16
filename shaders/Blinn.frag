
#version 400 core
in vec3 worldPosition;
in vec3 worldNormal;
out vec4 frag_color;
uniform vec3 viewPos;

void main(){
    vec3 objectColor = vec3(0.385, 0.647, 0.812);

    vec3 lightPosition = vec3(0.0, 0.0, 0.0);
    vec3 lightDir = normalize(lightPosition - worldPosition);
    
    vec3 norm = normalize(worldNormal);
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 reflectDir = reflect(-lightDir,norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 ambient = vec3(0.1, 0.1, 0.1) * objectColor;
   
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);

    float diff = max(dot(norm, lightDir), 0.0);
    
    
    vec3 result = ambient + (diff * objectColor) + (spec * vec3(1.0 ,1.0 ,1.0 ));
    frag_color = vec4(result,1.0);
}