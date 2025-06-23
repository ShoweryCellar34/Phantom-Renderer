#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(binding = 0) uniform sampler2D inputImage;
layout(rgba32f, binding = 1) uniform image2D outputImage;

const float offset = 1.0 / 300.0;

void main() {
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec2 uv = vec2(pixelCoord) / vec2(imageSize(outputImage));

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec4 samples[9];
    for(int i = 0; i < 9; i++) {
        samples[i] = texture(inputImage, uv + offsets[i]);
    }

    vec4 finalSample = vec4(0.0);
    for(int i = 0; i < 9; i++) {
        finalSample += samples[i] * kernel[i];
    }

    imageStore(outputImage, pixelCoord, finalSample);
}
