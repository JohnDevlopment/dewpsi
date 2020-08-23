#include "Dewpsi_WhichOS.h"
#include "Dewpsi_Application.h"
#include "Dewpsi_String.h"

#include <fstream>
#include <sstream>
#include <string>
#include <utility>

#define PD_GL_FUNC_DEFS
#include "Dewpsi_GLFuncs.h"

static unsigned int g_uiErrorLength = 500U;
static std::unique_ptr<char[]> g_cpError(new char[g_uiErrorLength]);
static unsigned int g_uiProgram = 0;

using namespace Dewpsi_ImGui_Impl;

static constexpr Dewpsi::StaticString ShaderStrings[] = {
    "vertex",
    "fragment",
    "geometry"
};

namespace Dewpsi_ImGui_Impl {



}

static void BackupState(OpenGLState* state)
{
    if (! state)
    {
        PD_BADPARAM("state");
        PD_CORE_ERROR("{0}: must be a non-null pointer", Dewpsi::GetError());
        return;
    }

    // last active texture unit
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*) &state->lastActiveTexture);
    glActiveTexture(GL_TEXTURE0);

    // program pipeline
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &state->program);

    // last bound texture
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*) &state->texture);

    // bound sampler object
#ifdef GL_SAMPLER_BINDING
    glGetIntegerv(GL_SAMPLER_BINDING, (GLint*) &state->lastSampler);
#endif

    // array buffer
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*) &state->lastArrayBuffer);

    // get polygon mode
#ifdef GL_POLYGON_MODE
    glGetIntegerv(GL_POLYGON_MODE, (GLint*) state->lastPolygonMode);
#endif

    // viewport
    glGetIntegerv(GL_VIEWPORT, state->viewport);

    // scissor box
    glGetIntegerv(GL_SCISSOR_BOX, state->scissorBox);

    // source blend function for RGB
    glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&state->lastSourceRGB);

    // destination blend function for RGB
    glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&state->lastDestRGB);

    // source blend function for alpha
    glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&state->lastSourceAlpha);

    // destination blend function for alpha
    glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&state->lastDestAlpha);

    // last blend equation of RGB
    glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&state->lastBlendEqRGB);

    // last blend equation of alpha
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&state->lastBlendEqAlpha);

    // boolean flags
    state->isBlend = glIsEnabled(GL_BLEND);
    state->isCullFace = glIsEnabled(GL_CULL_FACE);
    state->isDepthTest = glIsEnabled(GL_DEPTH_TEST);
    state->isScissorTest = glIsEnabled(GL_SCISSOR_TEST);
}

static void RestoreState(const OpenGLState* state)
{
    glUseProgram(state->program);
    glBindTexture(GL_TEXTURE_2D, state->texture);
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, state->lastSampler);
#endif
    glActiveTexture(state->lastActiveTexture);
    glBindVertexArray(state->lastArrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, state->lastArrayBuffer);
    glBlendEquationSeparate(state->lastBlendEqRGB, state->lastBlendEqAlpha);
    glBlendFuncSeparate(state->lastSourceRGB, state->lastDestRGB, state->lastSourceAlpha, state->lastDestAlpha);

    if (state->isBlend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    if (state->isCullFace)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    if (state->isDepthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (state->isScissorTest)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);

#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, (GLenum) state->lastPolygonMode[0]);
#endif

    glViewport(state->viewport[0], state->viewport[1], (GLsizei) state->viewport[2], (GLsizei) state->viewport[3]);
    glScissor(state->scissorBox[0], state->scissorBox[1], (GLsizei) state->scissorBox[2], (GLsizei) state->scissorBox[3]);
}

namespace Dewpsi {

bool OpenGL_InitShaders(const char* shaderFile)
{
    ShaderProgramSource sources = ParseShaderFile(shaderFile);
    g_uiProgram = CreateShader(sources);

    return g_uiProgram ? true : false;
}

void OpenGL_DeInitShaders()
{
    glDeleteProgram(g_uiProgram);
}

PDuint OpenGL_GetShader()
{
    return g_uiProgram;
}

void OpenGL_BackupState(OpenGLState* state)
{
    BackupState(state);
}

void OpenGL_RestoreState(const OpenGLState* state)
{
    RestoreState(state);
}

}
