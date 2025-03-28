#include <PR/cameraInternal.h>

#include <PR/error.h>

void i_prCameraGetDimensions(prCamera* camera) {
    int viewportSize[4];
    camera->context->GetIntegerv(GL_VIEWPORT, viewportSize);
    camera->viewportWidth = viewportSize[2];
    camera->viewportHeight = viewportSize[3];

    prLogEvent(PR_OPGL_EVENT, PR_LOG_TRCE, "Queried viewport dimentions {%i, %i, %i, %i}", viewportSize[0], viewportSize[1], viewportSize[2], viewportSize[3]);
}
