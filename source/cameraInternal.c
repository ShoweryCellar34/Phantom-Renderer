#include <PR/cameraInternal.h>

void i_prCameraGetDimensions(prCamera* camera) {
    int viewportSize[4];
    camera->context->GetIntegerv(GL_VIEWPORT, viewportSize);
    camera->viewportWidth = viewportSize[2];
    camera->viewportHeight = viewportSize[3];
}
