#version 460 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VERTEX_OUT {
    vec3 fragmentPosition;
    vec2 textureCoordinates;
    mat3 TBN;
    vec4 fragmentPositionLightSpace;
} vertexOut[];

out GEOMETRY_OUT {
    vec3 fragmentPosition;
    vec2 textureCoordinates;
    mat3 TBN;
    vec4 fragmentPositionLightSpace;
} geometryOut;

void main() {
    for(int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        geometryOut.fragmentPosition = vertexOut[i].fragmentPosition;
        geometryOut.textureCoordinates = vertexOut[i].textureCoordinates;
        geometryOut.TBN = vertexOut[i].TBN;
        geometryOut.fragmentPositionLightSpace = vertexOut[i].fragmentPositionLightSpace;
        EmitVertex();
    }
    EndPrimitive();
}
