#pragma once

#include <glad/gl.h>

// State categories
typedef enum prStateCategory {
    PR_STATE_BLEND,
    PR_STATE_CULL_FACE,
    PR_STATE_DEPTH_TEST,
    PR_STATE_STENCIL_TEST,
    PR_STATE_SCISSOR_TEST,
    PR_STATE_VIEWPORT
} prStateCategory;

typedef struct prStateData {
    GladGLContext* context;
    
    // Blending state
    GLboolean blendEnabled;
    GLenum blendSrcRGB;
    GLenum blendDstRGB;
    GLenum blendSrcAlpha;
    GLenum blendDstAlpha;
    GLenum blendEquationRGB;
    GLenum blendEquationAlpha;
    
    // Depth state
    GLboolean depthTestEnabled;
    GLenum depthFunc;
    GLboolean depthMask;
    
    // Face culling state
    GLboolean cullFaceEnabled;
    GLenum cullFaceMode;
    GLenum frontFace;
    
    // Stencil state
    GLboolean stencilTestEnabled;
    GLenum stencilFunc;
    GLint stencilRef;
    GLuint stencilMask;
    GLenum stencilFail;
    GLenum stencilPassDepthFail;
    GLenum stencilPassDepthPass;
    
    // Viewport state
    GLint viewportX;
    GLint viewportY;
    GLsizei viewportWidth;
    GLsizei viewportHeight;
} prStateData;

// Core state management
prStateData* prStateCreate();
void prStateDestroy(prStateData* state);
void prStateLinkContext(prStateData* state, GladGLContext* context);

// State capture and apply
void prStateCaptureCurrent(prStateData* state);
void prStateApply(prStateData* state);

// Enable/Disable capabilities
void prStateEnable(prStateData* state, prStateCategory capability);
void prStateDisable(prStateData* state, prStateCategory capability);
GLboolean prStateIsEnabled(prStateData* state, prStateCategory capability);

// Blending configuration
void prStateBlendFunc(prStateData* state, GLenum srcRGB, GLenum dstRGB);
void prStateBlendFuncSeparate(prStateData* state, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void prStateBlendEquation(prStateData* state, GLenum modeRGB);
void prStateBlendEquationSeparate(prStateData* state, GLenum modeRGB, GLenum modeAlpha);

// Depth configuration
void prStateDepthFunc(prStateData* state, GLenum func);
void prStateDepthMask(prStateData* state, GLboolean flag);

// Face culling configuration
void prStateCullFace(prStateData* state, GLenum mode);
void prStateFrontFace(prStateData* state, GLenum mode);

// Stencil configuration
void prStateStencilFunc(prStateData* state, GLenum func, GLint ref, GLuint mask);
void prStateStencilOp(prStateData* state, GLenum fail, GLenum zfail, GLenum zpass);
void prStateStencilMask(prStateData* state, GLuint mask);

// Viewport configuration
void prStateViewport(prStateData* state, GLint x, GLint y, GLsizei width, GLsizei height);
