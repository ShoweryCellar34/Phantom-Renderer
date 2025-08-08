#version 460 core
out vec4 fragmentColor;

layout(location = 0) uniform samplerCube skybox;

in vec3 textureCoordinates;

void main() {
    fragmentColor = texture(skybox, textureCoordinates);
}
