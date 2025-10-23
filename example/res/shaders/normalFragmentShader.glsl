#version 450 core
layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 brightColor;

in GEOMETRY_OUT {
    vec2 textureCoordinates;
    mat3 TBN;
} geometryOut;

struct Material {
    sampler2D normal;
};
uniform Material material;

void main() {
    vec3 normal = texture(material.normal, geometryOut.textureCoordinates).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(geometryOut.TBN * normal);

    brightColor = vec4(0.0, 0.0, 0.0, 1.0);
    fragmentColor = vec4((normal + 1.0) / 2.0, 1.0);
}
