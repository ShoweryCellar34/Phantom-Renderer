#version 460 core
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputNormal;

uniform mat4 view;
uniform mat4 translation;

out VERTEXOUT {
    vec3 normal;
} vertexOut;

void main() {
    mat3 normalMatrix = mat3(transpose(inverse(view * translation)));
    vertexOut.normal = vec3(vec4(normalMatrix * inputNormal, 0.0));
    gl_Position = view * translation * vec4(inputPosition, 1.0); 
}
