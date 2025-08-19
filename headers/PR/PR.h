#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cglm/struct.h>

#include <stb_image.h>
#include <stb_image_write.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "camera.h"
#include "computeShader.h"
#include "cubeMap.h"
#include "defines.h"
#include "framebuffer.h"
#include "helperStuffs.h"
#include "logger.h"
#include "memory.h"
#include "mesh.h"
#include "renderbuffer.h"
#include "shader.h"
#include "texture.h"
#include "textureMultisampled.h"
#include "window.h"

#ifdef __cplusplus
}
#endif
