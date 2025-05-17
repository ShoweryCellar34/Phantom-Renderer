#pragma once

#include <stdint.h>
#include <stddef.h>

typedef int8_t prLogLevel_t;
typedef int8_t prEvent_t;

#define PR_MAXSTR_LEN        1024
#define PR_CUBE_MAP_SIDES    6 // This is just for taking the place of a magic number

#define PR_EVENT_MEMORY      0
#define PR_EVENT_DATA        1
#define PR_EVENT_OPENGL      2
#define PR_EVENT_USER        3

#define PR_LOG_FATAL         4
#define PR_LOG_ERROR         5
#define PR_LOG_WARNING       6
#define PR_LOG_INFO          7
#define PR_LOG_TRACE         8

#define PR_WRAPPING_REPEAT               GL_REPEAT
#define PR_WRAPPING_REPEAT_MIRRORED      GL_MIRRORED_REPEAT
#define PR_WRAPPING_EDGE                 GL_CLAMP_TO_EDGE
#define PR_WRAPPING_COLOR                GL_CLAMP_TO_BORDER

#define PR_FILTER_LINEAR                 GL_LINEAR
#define PR_FILTER_NEAREST                GL_NEAREST

#define PR_ACCESS_READ_ONLY              GL_READ_ONLY
#define PR_ACCESS_WRITE_ONLY             GL_WRITE_ONLY
#define PR_ACCESS_READ_WRITE             GL_READ_WRITE

#define PR_FORMAT_R                      GL_RED
#define PR_FORMAT_G                      GL_GREEN
#define PR_FORMAT_B                      GL_BLUE
#define PR_FORMAT_A                      GL_ALPHA
#define PR_FORMAT_RGB                    GL_RGB
#define PR_FORMAT_RGBA                   GL_RGBA
#define PR_FORMAT_STENCIL                GL_STENCIL_INDEX
#define PR_FORMAT_DEPTH                  GL_DEPTH_COMPONENT
#define PR_FORMAT_DEPTH_STENCIL          GL_DEPTH24_STENCIL8
#define PR_FORMAT_AUTO                   GL_2D // Just a random format that can be used as a value for PR_FORMAT_AUTO

#define PR_BUFFER_BIT_COLOR              GL_COLOR_BUFFER_BIT
#define PR_BUFFER_BIT_DEPTH              GL_DEPTH_BUFFER_BIT  
#define PR_BUFFER_BIT_STENCIL            GL_STENCIL_BUFFER_BIT
