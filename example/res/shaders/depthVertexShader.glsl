#version 460 core
layout(location = 0) in vec3 inputPosition;

uniform mat4 lightSpaceMatrix;
uniform mat4 translation;

void main() {
    gl_Position = lightSpaceMatrix * translation * vec4(inputPosition, 1.0);
}
