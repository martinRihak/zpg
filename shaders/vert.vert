#version 450 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;   
out vec3 worldPosition;
out vec3 worldNormal;
layout(location=0) in vec3 vp;
layout(location=1) in vec3 vn;

void main(){
    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
    worldPosition = vec3(modelMatrix * vec4(vp, 1.0));
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * vn);
}