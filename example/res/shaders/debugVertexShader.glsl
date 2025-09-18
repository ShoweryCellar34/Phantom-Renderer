#version 460 core
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec2 inputTextureCoordinates;
layout(location = 2) in vec3 inputTangent;
layout(location = 3) in vec3 inputNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 translation;
uniform mat4 lightSpaceMatrix;

out VERTEX_OUT {
    vec3 fragmentPosition;
    vec2 textureCoordinates;
    mat3 TBN;
    vec4 fragmentPositionLightSpace;
} vertexOut;

void main() {
    gl_Position = projection * view * translation * vec4(inputPosition, 1.0);
    vertexOut.fragmentPosition = vec3(translation * vec4(inputPosition, 1.0));
    vertexOut.textureCoordinates = inputTextureCoordinates;
    vec3 T = normalize(vec3(translation * vec4(inputTangent, 0.0)));
    vec3 N = normalize(vec3(translation * vec4(inputNormal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    vertexOut.TBN = mat3(T, B, N);
    vertexOut.fragmentPositionLightSpace = lightSpaceMatrix * vec4(vertexOut.fragmentPosition, 1.0);
}
