#version 330 core

in vec3 pos;
out vec4 frag_color;
void main(){
    frag_color = vec4(pos + 0.5,1.0);
}