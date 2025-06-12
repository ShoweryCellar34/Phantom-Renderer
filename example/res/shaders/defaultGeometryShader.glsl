#version 460 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VERTEX_OUT {
    vec3 fragmentPosition;
    vec3 normals;
    vec2 textureCoordinates;
    vec4 fragmentPositionLightSpace;
} vertexOut[];

out GEOMETRY_OUT {
    vec3 fragmentPosition;
    vec3 normals;
    vec2 textureCoordinates;
    vec4 fragmentPositionLightSpace;
} geometryOut;

void main() {
    for(int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        geometryOut.fragmentPosition = vertexOut[i].fragmentPosition;
        geometryOut.normals = vertexOut[i].normals;
        geometryOut.textureCoordinates = vertexOut[i].textureCoordinates;
        geometryOut.fragmentPositionLightSpace = vertexOut[i].fragmentPositionLightSpace;
        EmitVertex();
    }
    EndPrimitive();
}
