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

void prCameraSetUpdateFunction(prCamera* camera, PRCameraUpdateFunction updateFunction) {
    camera->updateFunction = updateFunction;
}

void prCameraUpdate(prCamera* camera, vec3s position, vec3s rotation, vec3s worldUp, float FOV, float closePlane, float farPlane) {
    camera->position = position;

    camera->front.x = cos(rotation.x) * cos(rotation.y);
    camera->front.y = sin(rotation.y);
    camera->front.z = sin(rotation.x) * cos(rotation.y);
    camera->front = glms_normalize(camera->front);

    vec3s right = glms_normalize(glms_cross(camera->front, worldUp));

    camera->up = glms_normalize(glms_cross(right, camera->front));

    camera->up.x = camera->up.x * cos(rotation.z) + right.x * sin(rotation.z);
    camera->up.y = camera->up.y * cos(rotation.z) + right.y * sin(rotation.z);
    camera->up.z = camera->up.z * cos(rotation.z) + right.z * sin(rotation.z);
    camera->up = glms_normalize(camera->up);

    vec3s target = glms_vec3_add(position, camera->front);
    camera->view = glms_mat4_identity();
    camera->view = glms_lookat(position, target, camera->up);

    camera->projection = glms_perspective(FOV, (float)camera->viewportWidth / (float)camera->viewportHeight, closePlane, farPlane);

    if(camera->updateFunction) {
        camera->updateFunction(camera);
    }
}

void prCameraUpdateDimentions(prCamera* camera) {
    if(camera->context) {
        i_prCameraGetDimensions(camera);
    }
}
