#pragma once

typedef struct prRenderBufferData prRenderBufferData;

void i_prRenderBufferCreateOnGPU(prRenderBufferData* renderBuffer);

void i_prRenderBufferDestroyOnGPU(prRenderBufferData* renderBuffer);

void i_prRenderBufferUpdateOnGPU(prRenderBufferData* renderBuffer);
