#version 330 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;   
out vec3 vertexColor;
layout(location=0) in vec3 vp;
layout(location=1) in vec3 vn;
void main(){
    vertexColor = vp;
    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
}