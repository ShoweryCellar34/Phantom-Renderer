#version 460 core
out vec4 fragmentColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

in vec2 textureCoordinates;

void main() {
    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, textureCoordinates).rgb;
    vec3 bloomColor = texture(bloomBlur, textureCoordinates).rgb;
    hdrColor += bloomColor;

    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    result = pow(result, vec3(1.0 / gamma));
    fragmentColor = vec4(result, 1.0);
}
