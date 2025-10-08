#version 450 core
out vec4 fragmentColor;

struct Material {
    sampler2D diffuse;
};
uniform Material material;

in vec2 textureCoordinates;

void main() {
    fragmentColor = texture(material.diffuse, textureCoordinates);
}
