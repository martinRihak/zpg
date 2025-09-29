#version 330 core

layout(location=0) in vec3 vp;
out vec3 pos;
void main(){
    pos = vec3(vp.x,vp,y,0.5);
    gl_Position = vec4(vp,1.0);
}