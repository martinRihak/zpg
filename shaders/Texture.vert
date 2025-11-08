#version 450 core
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vt;
layout(location = 3) in vec3 ts;  
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out vec2 texCoords;
out vec3 worldPosition;
out vec3 worldNormal;
void main () {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (vp, 1.0);
    worldPosition = vec3(modelMatrix * vec4(vp, 1.0));
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * vn);
    texCoords=vt;
}