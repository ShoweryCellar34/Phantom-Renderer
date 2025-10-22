#version 450 core
layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 brightColor;

in vec3 geomColor;

void main() {
    vec3 result = geomColor;
    float brightness = dot(result.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        brightColor = vec4(result.rgb, 1.0);
    } else {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    fragmentColor = vec4(result, 1.0);
}
