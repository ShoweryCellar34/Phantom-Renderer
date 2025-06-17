#version 460 core
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec2 inputTextureCoordinates;

out vec2 textureCoordinates;

void main() {
    vec4 pos = vec4(inputPosition, 1.0);
    pos.xy = pos.xy * 2.0 - 1.0;
    gl_Position = pos;
    textureCoordinates = inputTextureCoordinates;
}
