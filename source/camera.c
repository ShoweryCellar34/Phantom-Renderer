#include <PR/camera.h>

#include <math.h>
#include <PR/memory.h>
#include <PR/cameraInternal.h>

prCamera* prCameraCreate() {
    prCamera* camera = prCalloc(1, sizeof(prCamera));

    camera->up = (vec3s){0.0f, 1.0f, 0.0f};

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

void prCameraUpdate(prCamera* camera, vec3s position, vec3s rotation, float FOV, float closePlane, float farPlane) {
    camera->position = position;

    // float roll = (rotation[2] * M_PI) / 180.0;

    // mat4 rollMatrix;
    // glm_mat4_identity(rollMatrix);
    // glm_rotate(rollMatrix, roll, camera->front);
    // glm_mat4_mulv3(rollMatrix, camera->up, 0.0f, camera->up);

    camera->front.x = cos(rotation.x) * cos(rotation.y);
    camera->front.y = sin(rotation.y);
    camera->front.z = sin(rotation.x) * cos(rotation.y);

    glms_normalize(camera->front);
    vec3s temp = glms_vec3_add(position, camera->front);

    camera->view = glms_mat4_identity();
    camera->view = glms_lookat(position, temp, camera->up);

    camera->projection = glms_perspective(FOV, (float)camera->viewportWidth / (float)camera->viewportHeight, closePlane, farPlane);
}

void prCameraUpdateDimentions(prCamera* camera) {
    if(camera->context) {
        i_prCameraGetDimensions(camera);
    }
}
