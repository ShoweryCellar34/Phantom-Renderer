#pragma once

#include <stdint.h>
#include <stddef.h>

typedef int8_t prLogLevel_t;
typedef int8_t prEvent_t;

#define PR_MAXSTR_LEN                           1024
#define PR_MAX_LOG_STREAMS                      8
#define PR_CUBE_MAP_SIDES                       6 // This is just for taking the place of a magic number
#define PR_MAX_VERTEX_ATTRIBUTES                16
#define PR_MAX_FRAMEBUFFER_COLOR_ATTACHMENTS    8
#define PR_MIN_SAMPLES                          4
#define PR_MAX_SAMPLES                          4

#define PR_EVENT_MEMORY                         0
#define PR_EVENT_DATA                           1
#define PR_EVENT_OPENGL                         2
#define PR_EVENT_USER                           3

#define PR_LOG_FATAL                            4
#define PR_LOG_ERROR                            5
#define PR_LOG_WARNING                          6
#define PR_LOG_INFO                             7
#define PR_LOG_TRACE                            8

#define PR_NONE                                 GL_NONE
#define PR_FALSE                                GL_FALSE
#define PR_TRUE                                 GL_TRUE
#define PR_FLOAT                                GL_FLOAT

#define PR_WRAPPING_REPEAT                      GL_REPEAT
#define PR_WRAPPING_REPEAT_MIRRORED             GL_MIRRORED_REPEAT
#define PR_WRAPPING_EDGE                        GL_CLAMP_TO_EDGE
#define PR_WRAPPING_BORDER                      GL_CLAMP_TO_BORDER

#define PR_FILTER_LINEAR                        GL_LINEAR
#define PR_FILTER_NEAREST                       GL_NEAREST

#define PR_FILTER_LINEAR_MIPMAP_LINEAR          GL_LINEAR_MIPMAP_LINEAR
#define PR_FILTER_LINEAR_MIPMAP_NEAREST         GL_LINEAR_MIPMAP_NEAREST
#define PR_FILTER_NEAREST_MIPMAP_NEAREST        GL_NEAREST_MIPMAP_NEAREST
#define PR_FILTER_NEAREST_MIPMAP_LINEAR         GL_NEAREST_MIPMAP_LINEAR

#define PR_ACCESS_READ_ONLY                     GL_READ_ONLY
#define PR_ACCESS_WRITE_ONLY                    GL_WRITE_ONLY
#define PR_ACCESS_READ_WRITE                    GL_READ_WRITE

#define PR_FORMAT_R                             GL_RED
#define PR_FORMAT_G                             GL_GREEN
#define PR_FORMAT_B                             GL_BLUE
#define PR_FORMAT_A                             GL_ALPHA
#define PR_FORMAT_RGB                           GL_RGB
#define PR_FORMAT_RGBA                          GL_RGBA
#define PR_FORMAT_SRGB                          GL_SRGB
#define PR_FORMAT_SRGBA                         GL_SRGB_ALPHA
#define PR_FORMAT_STENCIL                       GL_STENCIL_INDEX
#define PR_FORMAT_DEPTH                         GL_DEPTH_COMPONENT
#define PR_FORMAT_DEPTH_STENCIL                 GL_DEPTH24_STENCIL8
#define PR_FORMAT_AUTO                          GL_2D // Just a random format that can be used as a value for PR_FORMAT_AUTO
#define PR_FORMAT_SRGB_AUTO                     GL_3D // Just a random format that can be used as a value for PR_FORMAT_SRGB_AUTO

#define PR_BUFFER_BIT_COLOR                     GL_COLOR_BUFFER_BIT
#define PR_BUFFER_BIT_DEPTH                     GL_DEPTH_BUFFER_BIT  
#define PR_BUFFER_BIT_STENCIL                   GL_STENCIL_BUFFER_BIT

#define PR_COLOR_ATTACHMENT_0                   GL_COLOR_ATTACHMENT0
#define PR_COLOR_ATTACHMENT_1                   GL_COLOR_ATTACHMENT1
#define PR_COLOR_ATTACHMENT_2                   GL_COLOR_ATTACHMENT2
#define PR_COLOR_ATTACHMENT_3                   GL_COLOR_ATTACHMENT3
#define PR_COLOR_ATTACHMENT_4                   GL_COLOR_ATTACHMENT4
#define PR_COLOR_ATTACHMENT_5                   GL_COLOR_ATTACHMENT5
#define PR_COLOR_ATTACHMENT_6                   GL_COLOR_ATTACHMENT6
#define PR_COLOR_ATTACHMENT_7                   GL_COLOR_ATTACHMENT7
#define PR_COLOR_ATTACHMENT_8                   GL_COLOR_ATTACHMENT8
#define PR_COLOR_ATTACHMENT_9                   GL_COLOR_ATTACHMENT9
#define PR_COLOR_ATTACHMENT_10                  GL_COLOR_ATTACHMENT10
#define PR_COLOR_ATTACHMENT_11                  GL_COLOR_ATTACHMENT11
#define PR_COLOR_ATTACHMENT_12                  GL_COLOR_ATTACHMENT12
#define PR_COLOR_ATTACHMENT_13                  GL_COLOR_ATTACHMENT13
#define PR_COLOR_ATTACHMENT_14                  GL_COLOR_ATTACHMENT14
#define PR_COLOR_ATTACHMENT_15                  GL_COLOR_ATTACHMENT15
#define PR_COLOR_ATTACHMENT_16                  GL_COLOR_ATTACHMENT16
#define PR_COLOR_ATTACHMENT_17                  GL_COLOR_ATTACHMENT17
#define PR_COLOR_ATTACHMENT_18                  GL_COLOR_ATTACHMENT18
#define PR_COLOR_ATTACHMENT_19                  GL_COLOR_ATTACHMENT19
