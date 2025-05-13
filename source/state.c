#include <PR/state.h>

#include <PR/memory.h>
#include <PR/logger.h>

prStateData* prStateCreate() {
    prStateData* state = prCalloc(1, sizeof(prStateData));

    return state;
}

void prStateDestroy(prStateData* state) {
    if(!state) {
        return;
    }
    prFree(state);
}

void prStateLinkContext(prStateData* state, GladGLContext* context) {
    if(!state) {
        return;
    }
    state->context = context;
    if(context) {
        prStateCaptureCurrent(state);
    }
}

void prStateCaptureCurrent(prStateData* state) {
    if(!state || !state->context) {
        return;
    }
    
    // Capture blend state
    state->blendEnabled = state->context->IsEnabled(GL_BLEND);
    state->context->GetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&state->blendSrcRGB);
    state->context->GetIntegerv(GL_BLEND_DST_RGB, (GLint*)&state->blendDstRGB);
    state->context->GetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&state->blendSrcAlpha);
    state->context->GetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&state->blendDstAlpha);
    state->context->GetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&state->blendEquationRGB);
    state->context->GetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&state->blendEquationAlpha);
    
    // Capture depth state
    state->depthTestEnabled = state->context->IsEnabled(GL_DEPTH_TEST);
    state->context->GetIntegerv(GL_DEPTH_FUNC, (GLint*)&state->depthFunc);
    state->context->GetBooleanv(GL_DEPTH_WRITEMASK, &state->depthMask);
    
    // Capture face culling state
    state->cullFaceEnabled = state->context->IsEnabled(GL_CULL_FACE);
    state->context->GetIntegerv(GL_CULL_FACE_MODE, (GLint*)&state->cullFaceMode);
    state->context->GetIntegerv(GL_FRONT_FACE, (GLint*)&state->frontFace);
    
    // Capture stencil state
    state->stencilTestEnabled = state->context->IsEnabled(GL_STENCIL_TEST);
    state->context->GetIntegerv(GL_STENCIL_FUNC, (GLint*)&state->stencilFunc);
    state->context->GetIntegerv(GL_STENCIL_REF, &state->stencilRef);
    state->context->GetIntegerv(GL_STENCIL_VALUE_MASK, (GLint*)&state->stencilMask);
    state->context->GetIntegerv(GL_STENCIL_FAIL, (GLint*)&state->stencilFail);
    state->context->GetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, (GLint*)&state->stencilPassDepthFail);
    state->context->GetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, (GLint*)&state->stencilPassDepthPass);
    
    // Capture viewport state
    GLint viewport[4];
    state->context->GetIntegerv(GL_VIEWPORT, viewport);
    state->viewportX = viewport[0];
    state->viewportY = viewport[1];
    state->viewportWidth = viewport[2];
    state->viewportHeight = viewport[3];
}

void prStateApply(prStateData* state) {
    if (!state || !state->context) return;
    
    // Apply blend state
    if (state->blendEnabled) state->context->Enable(GL_BLEND);
    else state->context->Disable(GL_BLEND);
    state->context->BlendFuncSeparate(state->blendSrcRGB, state->blendDstRGB, 
                                    state->blendSrcAlpha, state->blendDstAlpha);
    state->context->BlendEquationSeparate(state->blendEquationRGB, state->blendEquationAlpha);
    
    // Apply depth state
    if (state->depthTestEnabled) state->context->Enable(GL_DEPTH_TEST);
    else state->context->Disable(GL_DEPTH_TEST);
    state->context->DepthFunc(state->depthFunc);
    state->context->DepthMask(state->depthMask);
    
    // Apply face culling state
    if (state->cullFaceEnabled) state->context->Enable(GL_CULL_FACE);
    else state->context->Disable(GL_CULL_FACE);
    state->context->CullFace(state->cullFaceMode);
    state->context->FrontFace(state->frontFace);
    
    // Apply stencil state
    if (state->stencilTestEnabled) state->context->Enable(GL_STENCIL_TEST);
    else state->context->Disable(GL_STENCIL_TEST);
    state->context->StencilFunc(state->stencilFunc, state->stencilRef, state->stencilMask);
    state->context->StencilOp(state->stencilFail, state->stencilPassDepthFail, state->stencilPassDepthPass);
    
    // Apply viewport state
    state->context->Viewport(state->viewportX, state->viewportY, 
                           state->viewportWidth, state->viewportHeight);
}

void prStateEnable(prStateData* state, prStateCategory capability) {
    if (!state || !state->context) return;
    
    switch (capability) {
        case PR_STATE_BLEND:
            state->blendEnabled = GL_TRUE;
            state->context->Enable(GL_BLEND);
            break;
        case PR_STATE_CULL_FACE:
            state->cullFaceEnabled = GL_TRUE;
            state->context->Enable(GL_CULL_FACE);
            break;
        case PR_STATE_DEPTH_TEST:
            state->depthTestEnabled = GL_TRUE;
            state->context->Enable(GL_DEPTH_TEST);
            break;
        case PR_STATE_STENCIL_TEST:
            state->stencilTestEnabled = GL_TRUE;
            state->context->Enable(GL_STENCIL_TEST);
            break;
        case PR_STATE_SCISSOR_TEST:
            state->context->Enable(GL_SCISSOR_TEST);
            break;
    }
}

void prStateDisable(prStateData* state, prStateCategory capability) {
    if (!state || !state->context) return;
    
    switch (capability) {
        case PR_STATE_BLEND:
            state->blendEnabled = GL_FALSE;
            state->context->Disable(GL_BLEND);
            break;
        case PR_STATE_CULL_FACE:
            state->cullFaceEnabled = GL_FALSE;
            state->context->Disable(GL_CULL_FACE);
            break;
        case PR_STATE_DEPTH_TEST:
            state->depthTestEnabled = GL_FALSE;
            state->context->Disable(GL_DEPTH_TEST);
            break;
        case PR_STATE_STENCIL_TEST:
            state->stencilTestEnabled = GL_FALSE;
            state->context->Disable(GL_STENCIL_TEST);
            break;
        case PR_STATE_SCISSOR_TEST:
            state->context->Disable(GL_SCISSOR_TEST);
            break;
    }
}

GLboolean prStateIsEnabled(prStateData* state, prStateCategory capability) {
    if (!state || !state->context) return GL_FALSE;
    
    switch (capability) {
        case PR_STATE_BLEND:
            return state->blendEnabled;
        case PR_STATE_CULL_FACE:
            return state->cullFaceEnabled;
        case PR_STATE_DEPTH_TEST:
            return state->depthTestEnabled;
        case PR_STATE_STENCIL_TEST:
            return state->stencilTestEnabled;
        case PR_STATE_SCISSOR_TEST:
            return state->context->IsEnabled(GL_SCISSOR_TEST);
        default:
            return GL_FALSE;
    }
}

void prStateBlendFunc(prStateData* state, GLenum srcRGB, GLenum dstRGB) {
    if (!state || !state->context) return;
    state->blendSrcRGB = state->blendSrcAlpha = srcRGB;
    state->blendDstRGB = state->blendDstAlpha = dstRGB;
    state->context->BlendFunc(srcRGB, dstRGB);
}

void prStateBlendFuncSeparate(prStateData* state, GLenum srcRGB, GLenum dstRGB, 
                             GLenum srcAlpha, GLenum dstAlpha
) {
    if (!state || !state->context) return;
    state->blendSrcRGB = srcRGB;
    state->blendDstRGB = dstRGB;
    state->blendSrcAlpha = srcAlpha;
    state->blendDstAlpha = dstAlpha;
    state->context->BlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void prStateBlendEquation(prStateData* state, GLenum modeRGB) {
    if (!state || !state->context) return;
    state->blendEquationRGB = state->blendEquationAlpha = modeRGB;
    state->context->BlendEquation(modeRGB);
}

void prStateBlendEquationSeparate(prStateData* state, GLenum modeRGB, GLenum modeAlpha) {
    if (!state || !state->context) return;
    state->blendEquationRGB = modeRGB;
    state->blendEquationAlpha = modeAlpha;
    state->context->BlendEquationSeparate(modeRGB, modeAlpha);
}

void prStateDepthFunc(prStateData* state, GLenum func) {
    if (!state || !state->context) return;
    state->depthFunc = func;
    state->context->DepthFunc(func);
}

void prStateDepthMask(prStateData* state, GLboolean flag) {
    if (!state || !state->context) return;
    state->depthMask = flag;
    state->context->DepthMask(flag);
}

void prStateCullFace(prStateData* state, GLenum mode) {
    if (!state || !state->context) return;
    state->cullFaceMode = mode;
    state->context->CullFace(mode);
}

void prStateFrontFace(prStateData* state, GLenum mode) {
    if (!state || !state->context) return;
    state->frontFace = mode;
    state->context->FrontFace(mode);
}

void prStateStencilFunc(prStateData* state, GLenum func, GLint ref, GLuint mask) {
    if (!state || !state->context) return;
    state->stencilFunc = func;
    state->stencilRef = ref;
    state->stencilMask = mask;
    state->context->StencilFunc(func, ref, mask);
}

void prStateStencilOp(prStateData* state, GLenum fail, GLenum zfail, GLenum zpass) {
    if (!state || !state->context) return;
    state->stencilFail = fail;
    state->stencilPassDepthFail = zfail;
    state->stencilPassDepthPass = zpass;
    state->context->StencilOp(fail, zfail, zpass);
}

void prStateStencilMask(prStateData* state, GLuint mask) {
    if (!state || !state->context) return;
    state->stencilMask = mask;
    state->context->StencilMask(mask);
}

void prStateViewport(prStateData* state, GLint x, GLint y, GLsizei width, GLsizei height) {
    if (!state || !state->context) return;
    state->viewportX = x;
    state->viewportY = y;
    state->viewportWidth = width;
    state->viewportHeight = height;
    state->context->Viewport(x, y, width, height);
}
