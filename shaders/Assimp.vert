#version 450
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;   
out vec3 worldPosition;
out vec3 worldNormal;
layout(location = 0) in vec3 vp;  // position
layout(location = 1) in vec3 vn;  // normal
layout(location = 2) in vec2 vt;  // texture space
layout(location = 3) in vec3 ts;  // tangent space

out vec2 uv;

void main () {
	uv = vt;
    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
    worldPosition = vec3(modelMatrix * vec4(vp, 1.0));
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * vn);
}
