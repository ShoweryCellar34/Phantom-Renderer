#include <PR/camera.h>

#include <math.h>
#include <PR/memory.h>
#include <PR/cameraInternal.h>

prCamera* prCameraCreate() {
    prCamera* camera = prCalloc(0, sizeof(prCamera));

    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->up);

    return camera;
}

void prCameraDestroy(prCamera* camera) {
    prFree(camera);
}

void prCameraLinkContext(prCamera* camera, GladGLContext* context) {
    camera->context = context;
    if(camera->context) {
        i_prCameraGetDimensions(camera);
    }
}

void prCameraUpdate(prCamera* camera, vec3 position, vec3 rotation, float FOV, float closePlane, float farPlane) {
    glm_vec3_copy(position, camera->position);

    vec3 temp = {0.0f, 0.0f, 0.0f};

    // float roll = (rotation[2] * M_PI) / 180.0;

    // mat4 rollMatrix;
    // glm_mat4_identity(rollMatrix);
    // glm_rotate(rollMatrix, roll, camera->front);
    // glm_mat4_mulv3(rollMatrix, camera->up, 0.0f, camera->up);

    camera->front[0] = cos(rotation[0]) * cos(rotation[1]);
    camera->front[1] = sin(rotation[1]);
    camera->front[2] = sin(rotation[0]) * cos(rotation[1]);

    glm_normalize(camera->front);
    glm_vec3_add(position, camera->front, temp);

    glm_mat4_identity(camera->view);
    glm_lookat(position, temp, camera->up, camera->view);

    glm_perspective(FOV, (float)camera->viewportWidth/(float)camera->viewportHeight, closePlane, farPlane, camera->projection);
}

void prCameraUpdateDimentions(prCamera* camera) {
    if(camera->context) {
        i_prCameraGetDimensions(camera);
    }
}
