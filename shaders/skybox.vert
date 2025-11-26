#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vt;

out vec3 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrixSkyBox;
uniform mat4 projectMatrix;

void main() {
    uv = position;
    gl_Position = projectMatrix * viewMatrixSkyBox * modelMatrix * vec4(position, 1.0);
}
