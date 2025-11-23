#version 450 core
in vec3 uv;
out vec4 frag_color;

uniform samplerCube cubeTexture;

void main() {
   frag_color = texture(cubeTexture,uv);
}
