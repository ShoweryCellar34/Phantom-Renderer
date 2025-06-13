#version 460 core
out vec4 fragmentColor;

uniform sampler2D ambient;

in vec2 textureCoordinates;

void main() {
    fragmentColor = texture(ambient, textureCoordinates);
}
