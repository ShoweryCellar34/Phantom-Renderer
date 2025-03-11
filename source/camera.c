#include <PR/camera.h>

#include <math.h>
#include <PR/memory.h>
#include <PR/cameraInternal.h>

prCamera* prCameraCreate() {
    prCamera* camera = prMalloc(sizeof(prCamera));

    camera->context = NULL;
    glm_mat4_copy(GLM_MAT4_ZERO, camera->view);
    glm_mat4_copy(GLM_MAT4_ZERO, camera->projection);
    camera->viewportWidth = 0;
    camera->viewportHeight = 0;
    glm_vec3_zero(camera->front);

    return camera;
}

void prCameraDestroy(prCamera* camera) {
    prFree(camera);
}

void prCameraLink(prCamera* camera, GladGLContext* context) {
    camera->context = context;
    if(camera->context) {
        i_prCameraGetDimensions(camera);

        glm_perspective(45.0f, (float)camera->viewportWidth/(float)camera->viewportHeight, 0.1f, 100.0f, camera->projection);
    }
}

vec3 up = {0.0f, 1.0f, 0.0f};

void prCameraUpdate(prCamera* camera, vec3 position, vec3 rotation) {
    if(camera->context) {
        i_prCameraGetDimensions(camera);
    }

    vec3 temp = {0.0f, 0.0f, 0.0f};

    float yaw = (rotation[0] * M_PI) / 180.0;
    float pitch = (rotation[1] * M_PI) / 180.0;

    camera->front[0] = cos(yaw) * cos(pitch);
    camera->front[1] = sin(pitch);
    camera->front[2] = sin(yaw) * cos(pitch);

    glm_normalize(camera->front);
    glm_vec3_add(position, camera->front, temp);

    glm_mat4_identity(camera->view);
    glm_lookat(position, temp, up, camera->view);

    glm_perspective(45.0f, (float)camera->viewportWidth/(float)camera->viewportHeight, 0.1f, 100.0f, camera->projection);
}
