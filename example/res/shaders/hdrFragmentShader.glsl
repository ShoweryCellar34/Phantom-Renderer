#version 460 core
out vec4 fragmentColor;

uniform sampler2D ambient;

in vec2 textureCoordinates;

void main() {
    const float gamma = 2.0;
    const float exposure = 1.0;
    vec3 hdrColor = texture(ambient, textureCoordinates).rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));

    fragmentColor = vec4(mapped, 1.0);
}
