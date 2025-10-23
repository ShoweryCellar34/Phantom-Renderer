#version 450 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VERTEX_OUT {
    vec2 textureCoordinates;
    mat3 TBN;
} vertexOut[];

out GEOMETRY_OUT {
    vec2 textureCoordinates;
    mat3 TBN;
} geometryOut;

void main() {
    for(int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        geometryOut.textureCoordinates = vertexOut[i].textureCoordinates;
        geometryOut.TBN = vertexOut[i].TBN;
        EmitVertex();
    }
    EndPrimitive();
}
