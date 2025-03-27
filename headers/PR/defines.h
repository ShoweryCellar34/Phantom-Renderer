#pragma once

typedef int prLogLevel_t;
typedef int prError_t;

#define PR_MMRY_EVENT 0
#define PR_DATA_EVENT 1
#define PR_OPGL_EVENT 2
#define PR_USER_EVENT 3

#define PR_LOG_FTAL 0
#define PR_LOG_EROR 1
#define PR_LOG_WARN 2
#define PR_LOG_INFO 3
#define PR_LOG_TRCE 4

#define PR_WRAPPING_REPEAT GL_REPEAT
#define PR_WRAPPING_REPEAT_MIRRORED GL_MIRRORED_REPEAT
#define PR_WRAPPING_EDGE GL_CLAMP_TO_EDGE
#define PR_WRAPPING_COLOR GL_CLAMP_TO_BORDER
