#version 330 core

in vec3 vertexColor;
out vec4 frag_color;
void main(){
    frag_color = vec4(vertexColor,1.0);
}