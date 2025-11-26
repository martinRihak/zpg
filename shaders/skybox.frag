#version 450 core
in vec3 uv;
out vec4 frag_color;

uniform samplerCube cubeTexture;
uniform float time;
void main() {
   vec4 color = texture(cubeTexture,uv);
   frag_color = vec4(color.rgb * time , 1.0);
}
