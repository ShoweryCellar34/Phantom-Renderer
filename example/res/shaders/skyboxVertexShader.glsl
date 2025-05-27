#version 460 core
layout(location = 0) in vec3 inputPosition;

uniform mat4 translation;
uniform mat4 view;
uniform mat4 projection;

out vec3 textureCoordinates;

void main() {
    gl_Position = (projection * view * translation * vec4(inputPosition, 1.0)).xyww;
    textureCoordinates = inputPosition;
}
