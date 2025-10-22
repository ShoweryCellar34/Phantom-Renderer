#version 450 core
layout(location = 0) in vec3 inputPosition;
layout(location = 2) in vec3 inputTangent;
layout(location = 3) in vec3 inputNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 translation;

out VERTEX_OUT {
    mat3 TBN;
    vec3 worldPosition;
    vec3 normal;
} vertexOut;

void main() {
    // Compute and store world-space position and normal so the geometry shader can compute
    // a single face-normal line per triangle.
    vec4 worldPos4 = translation * vec4(inputPosition, 1.0);
    vec3 worldPos = vec3(worldPos4);
    vec3 T = normalize(vec3(translation * vec4(inputTangent, 0.0)));
    vec3 N = normalize(vec3(translation * vec4(inputNormal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    vertexOut.TBN = mat3(T, B, N);
    vertexOut.worldPosition = worldPos;
    vertexOut.normal = N;

    // Position for the rasterizer (we still output transformed position here)
    gl_Position = projection * view * worldPos4;
}
