#version 460 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float time;

in VERTEX_OUT {
    vec3 fragmentPosition;
    vec3 normals;
    vec2 textureCoordinates;
    vec3 wave;
} vertexOut[];

out GEOMETRY_OUT {
    vec3 fragmentPosition;
    vec3 normals;
    vec2 textureCoordinates;
} geometryOut;

void main() {
    for(int i = 0; i < 3; i++) {
        vec3 offset = vec3(vertexOut[i].wave * vec3(sin(time), sin(time + 2.094), sin(time + 4.188)));
        gl_Position = gl_in[i].gl_Position + vec4(offset, 1.0);
        geometryOut.fragmentPosition = vertexOut[i].fragmentPosition;
        geometryOut.normals = vertexOut[i].normals;
        geometryOut.textureCoordinates = vertexOut[i].textureCoordinates;
        EmitVertex();
    }
    EndPrimitive();
}
