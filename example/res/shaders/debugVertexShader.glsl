#version 460 core
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputNormals;
layout(location = 2) in vec2 inputTextureCoordinates;

uniform mat4 translation;
uniform mat4 view;
uniform mat4 projection;

out VERTEX_OUT {
    vec3 fragmentPosition;
    vec3 normals;
    vec2 textureCoordinates;
} vertexOut;

void main() {
    gl_Position = projection * view * translation * vec4(inputPosition, 1.0);
    vertexOut.fragmentPosition = vec3(translation * vec4(inputPosition, 1.0));
    vertexOut.normals = mat3(transpose(inverse(translation))) * inputNormals;
    vertexOut.textureCoordinates = inputTextureCoordinates;
}
