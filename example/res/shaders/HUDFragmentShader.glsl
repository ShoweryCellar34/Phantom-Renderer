#version 460 core
out vec4 fragmentColor;

in vec2 textureCoordinates;

uniform sampler2D ambient;

void main() {
    fragmentColor = texture(ambient, textureCoordinates);
}
