#ifndef PR_RENDER_BUFFER_INTERNAL_H
#define PR_RENDER_BUFFER_INTERNAL_H

typedef struct prRenderBufferData prRenderBufferData;

void i_prRenderBufferCreateOnGPU(prRenderBufferData* renderBuffer);

void i_prRenderBufferDestroyOnGPU(prRenderBufferData* renderBuffer);

void i_prRenderBufferUpdateOnGPU(prRenderBufferData* renderBuffer);

#endif // PR_RENDER_BUFFER_INTERNAL_H
