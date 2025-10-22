#version 450 core
layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

in VERTEX_OUT {
    mat3 TBN;
    vec3 worldPosition;
    vec3 normal;
} vertexOut[];

uniform mat4 projection;
uniform mat4 view;
uniform float normalLength = 1.0;
uniform vec3 normalColor = vec3(1.0, 1.0, 0.0);

out vec3 geomColor;

void main() {
    // Compute triangle centroid_ (world space)
    vec3 p0 = vertexOut[0].worldPosition;
    vec3 p1 = vertexOut[1].worldPosition;
    vec3 p2 = vertexOut[2].worldPosition;
    vec3 centroid_ = (p0 + p1 + p2) / 3.0;

    // Compute face normal by averaging the per-vertex normals taken from the TBN
    // third column. In the vertex shader TBN was built as mat3(T, B, N) so column 2 is N.
    vec3 n0 = normalize(vertexOut[0].TBN[2]);
    vec3 n1 = normalize(vertexOut[1].TBN[2]);
    vec3 n2 = normalize(vertexOut[2].TBN[2]);
    vec3 faceNormal = normalize(n0 + n1 + n2);

    // Fallback: if the averaged normal is degenerate, compute geometric normal.
    if(length(faceNormal) < 1e-6) {
        faceNormal = normalize(cross(p1 - p0, p2 - p0));
    }

    geomColor = normalColor;

    // Emit start vertex at centroid_
    vec4 clipStart = projection * view * vec4(centroid_, 1.0);
    gl_Position = clipStart;
    EmitVertex();

    // Emit end vertex at centroid_ + normal * length
    vec4 clipEnd = projection * view * vec4(centroid_ + faceNormal * normalLength, 1.0);
    gl_Position = clipEnd;
    EmitVertex();

    EndPrimitive();
}
