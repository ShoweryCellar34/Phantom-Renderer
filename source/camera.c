#include <PR/camera.h>

#include <PR/memory.h>
#include <PR/cameraInternal.h>

prCamera* prCameraCreate() {
    prCamera* camera = prMalloc(sizeof(prCamera));

    camera->context = NULL;
    glm_mat4_copy(GLM_MAT4_ZERO, camera->view);
    glm_mat4_copy(GLM_MAT4_ZERO, camera->projection);
    camera->viewportWidth = 0;
    camera->viewportHeight = 0;

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
vec3 front = {0.0f, 0.0f, -1.0f};

void prCameraUpdate(prCamera* camera, vec3 position) {
    if(camera->context) {
        i_prCameraGetDimensions(camera);
    }

    vec3 temp;
    glm_vec3_add(position, front, temp);

    glm_mat4_identity(camera->view);
    glm_lookat(position, temp, up, camera->view);

    glm_perspective(45.0f, (float)camera->viewportWidth/(float)camera->viewportHeight, 0.1f, 100.0f, camera->projection);
}
