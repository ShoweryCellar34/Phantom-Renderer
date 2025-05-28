#version 460 core
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputNormals;
layout(location = 2) in vec2 inputTextureCoordinates;
layout(location = 3) in vec3 inputWave;

uniform mat4 translation;
uniform mat4 view;
uniform mat4 projection;
uniform int width;
uniform int height;
uniform bool grass;

out VERTEX_OUT {
    vec3 fragmentPosition;
    vec3 normals;
    vec2 textureCoordinates;
    vec3 wave;
} vertexOut;

vec3 offset() {
    return vec3(gl_InstanceID % width / 10.0f, 0.0f, (gl_InstanceID / height) / 10.0f);
}

void main() {
    gl_Position = projection * view * translation * vec4(inputPosition + (grass ? offset() : vec3(0.0)), 1.0);
    vertexOut.fragmentPosition = vec3(translation * vec4(inputPosition, 1.0));
    vertexOut.normals = mat3(transpose(inverse(translation))) * inputNormals;
    vertexOut.textureCoordinates = inputTextureCoordinates;
    vertexOut.wave = inputWave;
}
