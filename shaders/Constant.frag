#version 400 core
in vec3 worldPosition;
in vec3 worldNormal;
out vec4 frag_color;
void main(){
    vec4 objectColor = vec4(0.385, 0.647, 0.812, 1.0);
    frag_color =  objectColor;}

