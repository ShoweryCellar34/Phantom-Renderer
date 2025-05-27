#version 460 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

uniform mat4 projection;

in VERTEXOUT {
    vec3 normal;
} vertexOut[];

const float MAGNITUDE = 0.2;

void GenerateLine(int index) {
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + vec4(vertexOut[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main() {
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}
