#include <PR/cameraInternal.h>

#include <glad/gl.h>
#include <PR/camera.h>
#include <PR/logger.h>

void i_prCameraGetDimensions(prCamera* camera) {
    int viewportSize[4];
    camera->context->GetIntegerv(GL_VIEWPORT, viewportSize);
    camera->viewportWidth = viewportSize[2];
    camera->viewportHeight = viewportSize[3];

    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prCameraGetDimensions: Queried viewport dimentions {%i, %i, %i, %i}", viewportSize[0], viewportSize[1], viewportSize[2], viewportSize[3]);
}
