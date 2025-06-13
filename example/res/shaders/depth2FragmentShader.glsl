#version 460 core

uniform vec3 lightPosition;
uniform float farPlane;

in vec4 fragmentPosition;

void main() {
    float lightDistance = length(fragmentPosition.xyz - lightPosition);
    lightDistance = lightDistance / farPlane;
    gl_FragDepth = lightDistance;
}
