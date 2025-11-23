#version 450 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;   
out vec3 worldPosition;
out vec3 worldNormal;
layout(location=0) in vec3 vp;
layout(location=1) in vec3 vn;
layout(location=2) in vec2 vt;

uniform float w = 500.0;
out vec2 texCoords;
void main(){
    texCoords = vt;
    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(vp * w , w);
    worldPosition = vec3(modelMatrix * vec4(vp, 1.0));
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * vn);
}