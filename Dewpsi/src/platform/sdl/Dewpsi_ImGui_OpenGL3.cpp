// dear imgui: Renderer for modern OpenGL with shaders / programmatic pipeline
// - Desktop GL: 2.x 3.x 4.x
// - Embedded GL: ES 2.0 (WebGL 1.0), ES 3.0 (WebGL 2.0)
// This needs to be used along with a Platform Binding (e.g. GLFW, SDL, Win32, custom..)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'GLuint' OpenGL texture identifier as void*/ImTextureID. Read the FAQ about ImTextureID!
//  [x] Renderer: Desktop GL only: Support for large meshes (64k+ vertices) with 16-bit indices.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2020-07-10: OpenGL: Added support for glad2 OpenGL loader.
//  2020-05-08: OpenGL: Made default GLSL version 150 (instead of 130) on OSX.
//  2020-04-21: OpenGL: Fixed handling of glClipControl(GL_UPPER_LEFT) by inverting projection matrix.
//  2020-04-12: OpenGL: Fixed context version check mistakenly testing for 4.0+ instead of 3.2+ to enable ImGuiBackendFlags_RendererHasVtxOffset.
//  2020-03-24: OpenGL: Added support for glbinding 2.x OpenGL loader.
//  2020-01-07: OpenGL: Added support for glbinding 3.x OpenGL loader.
//  2019-10-25: OpenGL: Using a combination of GL define and runtime GL version to decide whether to use glDrawElementsBaseVertex(). Fix building with pre-3.2 GL loaders.
//  2019-09-22: OpenGL: Detect default GL loader using __has_include compiler facility.
//  2019-09-16: OpenGL: Tweak initialization code to allow application calling ImGui_OpenGL_CreateFontsTexture() before the first NewFrame() call.
//  2019-05-29: OpenGL: Desktop GL only: Added support for large mesh (64K+ vertices), enable ImGuiBackendFlags_RendererHasVtxOffset flag.
//  2019-04-30: OpenGL: Added support for special ImDrawCallback_ResetRenderState callback to reset render state.
//  2019-03-29: OpenGL: Not calling glBindBuffer more than necessary in the render loop.
//  2019-03-15: OpenGL: Added a dummy GL call + comments in ImGui_OpenGL_Init() to detect uninitialized GL function loaders early.
//  2019-03-03: OpenGL: Fix support for ES 2.0 (WebGL 1.0).
//  2019-02-20: OpenGL: Fix for OSX not supporting OpenGL 4.5, we don't try to read GL_CLIP_ORIGIN even if defined by the headers/loader.
//  2019-02-11: OpenGL: Projecting clipping rectangles correctly using drawData->FramebufferScale to allow multi-viewports for retina display.
//  2019-02-01: OpenGL: Using GLSL 410 shaders for any version over 410 (e.g. 430, 450).
//  2018-11-30: Misc: Setting up io.BackendRendererName so it can be displayed in the About Window.
//  2018-11-13: OpenGL: Support for GL 4.5's glClipControl(GL_UPPER_LEFT) / GL_CLIP_ORIGIN.
//  2018-08-29: OpenGL: Added support for more OpenGL loaders: glew and glad, with comments indicative that any loader can be used.
//  2018-08-09: OpenGL: Default to OpenGL ES 3 on iOS and Android. GLSL version default to "#version 300 ES".
//  2018-07-30: OpenGL: Support for GLSL 300 ES and 410 core. Fixes for Emscripten compilation.
//  2018-07-10: OpenGL: Support for more GLSL versions (based on the GLSL version string). Added error output when shaders fail to compile/link.
//  2018-06-08: Misc: Extracted imgui_impl_opengl3.cpp/.h away from the old combined GLFW/SDL+OpenGL3 examples.
//  2018-06-08: OpenGL: Use drawData->DisplayPos and drawData->DisplaySize to setup projection matrix and clipping rectangle.
//  2018-05-25: OpenGL: Removed unnecessary backup/restore of GL_ELEMENT_ARRAY_BUFFER_BINDING since this is part of the VAO state.
//  2018-05-14: OpenGL: Making the call to glBindSampler() optional so 3.2 context won't fail if the function is a NULL pointer.
//  2018-03-06: OpenGL: Added const char* glsl_version parameter to ImGui_OpenGL_Init() so user can override the GLSL version e.g. "#version 150".
//  2018-02-23: OpenGL: Create the VAO in the render function so the setup can more easily be used with multiple shared GL context.
//  2018-02-16: Misc: Obsoleted the io.RenderDrawListsFn callback and exposed ImGui_ImplSdlGL3_RenderDrawData() in the .h file so you can call it yourself.
//  2018-01-07: OpenGL: Changed GLSL shader version from 330 to 150.
//  2017-09-01: OpenGL: Save and restore current bound sampler. Save and restore current polygon mode.
//  2017-05-01: OpenGL: Fixed save and restore of current blend func state.
//  2017-05-01: OpenGL: Fixed save and restore of current GL_ACTIVE_TEXTURE.
//  2016-09-05: OpenGL: Fixed save and restore of current scissor rectangle.
//  2016-07-29: OpenGL: Explicitly setting GL_UNPACK_ROW_LENGTH to reduce issues because SDL changes it. (#752)

//----------------------------------------
// OpenGL    GLSL      GLSL
// version   version   string
//----------------------------------------
//  2.0       110       "#version 110"
//  2.1       120       "#version 120"
//  3.0       130       "#version 130"
//  3.1       140       "#version 140"
//  3.2       150       "#version 150"
//  3.3       330       "#version 330 core"
//  4.0       400       "#version 400 core"
//  4.1       410       "#version 410 core"
//  4.2       420       "#version 410 core"
//  4.3       430       "#version 430 core"
//  ES 2.0    100       "#version 100"      = WebGL 1.0
//  ES 3.0    300       "#version 300 es"   = WebGL 2.0
//----------------------------------------

#include "Dewpsi_WhichOS.h"
#include "imgui.h"
#include "Dewpsi_Rect.h"
#include "Dewpsi_String.h"


#include <cstring>
#include <cstdio>

using namespace Dewpsi_ImGui_Impl;

static std::unique_ptr<char[]> g_ShaderPath;

static OpenGLData g_Data = {0};

bool ImGui_OpenGL_Init(const char* glslPath, const char* glslVersion)
{
    int iMajor, iMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &iMajor);
    glGetIntegerv(GL_MINOR_VERSION, &iMinor);
    g_Data.glVersion = static_cast<PDuint>(iMajor * 100 + iMinor * 10);
    
    {
        char* cpBuffer = nullptr;
        size_t szResult = Dewpsi::String::Copy((char*) &cpBuffer, glslPath, 100, PD_ALLOC);
        if (cpBuffer)
            g_ShaderPath.reset(cpBuffer);
        PD_CORE_ASSERT(szResult != PD_INVALID, "ERROR: ImGui_OpenGL_Init: Invalid code from Dewpsi::String::Copy : {0}", Dewpsi::GetError());
    }
    
    // setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_opengl";
    
    PD_CORE_INFO("OpenGL version {0}.{1}", iMajor, iMinor); // TODO: remove this line
    
    // store GLSL version string
    if (glslVersion == NULL)
        glslVersion = "#version 400";
    
    IM_ASSERT((int) std::strlen(glslVersion) + 2 < IM_ARRAYSIZE(g_Data.glslVersionString));
    std::strcpy(g_Data.glslVersionString, glslVersion);
    std::strcat(g_Data.glslVersionString, "\n");
    
    // dummy construct for debuggers
    const char* cpLoader = "GLAD";
    
    // dummy GL call
    int iCurrentTex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &iCurrentTex);
}

void ImGui_OpenGL_Shutdown()
{
    ImGui_OpenGL_DestroyDeviceObjects();
}

void ImGui_OpenGL_NewFrame()
{
    if (! g_Data.shader)
        ImGui_OpenGL_CreateDeviceObjects();
}

static void ImGui_OpenGL_SetupRenderState(ImDrawData* drawData, int fbWidth, int fbHeight, GLuint vao)
{
    // alpha and RGB blending method: additive blending
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_CULL_FACE);
    
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    // support for GL 4.5 rarely used glClipControl(GL_UPPER_LEFT)
    bool ClipOriginLowerLeft = true;
    
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
    GLenum eCurrentClipOrigin = 0;
    glGetIntegerv(GL_CLIP_ORIGIN, (GLint*)&eCurrentClipOrigin);
    if (eCurrentClipOrigin == GL_UPPER_LEFT)
        ClipOriginLowerLeft = false;
#endif
    
    // setup viewport, orthogonal projection matrix
    glViewport(0, 0, (GLsizei) fbWidth, (GLsizei) fbHeight);
    
    {
        float fL = drawData->DisplayPos.x;
        float fR = drawData->DisplayPos.x + drawData->DisplaySize.x;
        float fT = drawData->DisplayPos.y;
        float fB = drawData->DisplayPos.y + drawData->DisplaySize.y;
        if (! ClipOriginLowerLeft)
            Dewpsi::Swap(fT, fB);
        
        const float faOrthoProj[4][4] = {
            { 2.0f / (fR - fL),         0.0f,                   0.0f,   0.0f },
            { 0.0f,                     2.0f / (fT - fB),       0.0f,   0.0f },
            { 0.0f,                     0.0f,                  -1.0f,   0.0f },
            { (fR + fL) / (fL - fR),    (fT + fB) / (fB - fT),  0.0f,   1.0f }
        };
        
        glUseProgram(g_Data.shader);
        glUniform1i(g_Data.attribLocationTex, 0);
        glUniformMatrix4fv(g_Data.attribLocationProjMtx, 1, GL_FALSE, &faOrthoProj[0][0]);
#ifdef GL_SAMPLER_BINDING
        glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
#endif
    }
    
    glBindVertexArray(vao);

    // bind vertex/index buffers and setup attributes for ImDrawVert
    glBindBuffer(GL_ARRAY_BUFFER, g_Data.vboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Data.elementsHandle);
    
    glEnableVertexAttribArray(g_Data.attribLocationVtxPos);
    glEnableVertexAttribArray(g_Data.attribLocationVtxUV);
    glEnableVertexAttribArray(g_Data.attribLocationVtxColor);
    
    glVertexAttribPointer(g_Data.attribLocationVtxPos,      2, GL_FLOAT,            GL_FALSE,   sizeof(ImDrawVert), (GLvoid*) IM_OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(g_Data.attribLocationVtxUV,       2, GL_FLOAT,            GL_FALSE,   sizeof(ImDrawVert), (GLvoid*) IM_OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(g_Data.attribLocationVtxColor,    4, GL_UNSIGNED_BYTE,    GL_TRUE,    sizeof(ImDrawVert), (GLvoid*) IM_OFFSETOF(ImDrawVert, col));
}

// OpenGL3 Render function.
// (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(), but you can now call this directly from your main loop)
// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so.

/* OpenGL render function. Called directly from the main loop. This is made complicated by the fact that the GL state is backed up. */
void ImGui_OpenGL_RenderDrawData(ImDrawData* drawData)
{
    OpenGLState state;
    
    // avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int iWidth = static_cast<int>(drawData->DisplaySize.x * drawData->FramebufferScale.x);
    int iHeight = static_cast<int>(drawData->DisplaySize.y * drawData->FramebufferScale.y);
    
    if (iWidth <= 0 || iHeight <= 0)
        return;
    
    Dewpsi::OpenGL_BackupState(&state);
    
    /* Setup desired GL state.
    Recreate the VAO every time to allow multiple GL contexts to be renderered to (VAO are not shared among GL contexts). */
    GLuint uiVao = 0;
    ImGui_OpenGL_SetupRenderState(drawData, iWidth, iHeight, uiVao);
    
    // clipping vectors for the projection
    const ImVec2& clipOff = drawData->DisplayPos;
    const ImVec2& clipScale = drawData->FramebufferScale;
    
    // render command lists
    constexpr size_t szVert = sizeof(ImDrawVert);
    constexpr size_t szDraw = sizeof(ImDrawIdx);
    
    for (int n = 0; n < drawData->CmdListsCount; ++n)
    {
        const ImDrawList* const cmdList = drawData->CmdLists[n];
        
        // upload buffers
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) cmdList->VtxBuffer.Size * szVert, (const GLvoid*) cmdList->VtxBuffer.Data, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) cmdList->IdxBuffer.Size * szDraw, (const GLvoid*) cmdList->IdxBuffer.Data, GL_STREAM_DRAW);
        
        for (int cmdi = 0; cmdi < cmdList->CmdBuffer.Size; ++cmdi)
        {
            const ImDrawCmd* pCmd = &cmdList->CmdBuffer[cmdi];
            if (pCmd->UserCallback != nullptr)
            {
                /* User callback, registered via ImDrawList::AddCallback().
                (ImDrawCallback_ResetRenderState is a special value used by the user to request the renderer to reset render state.) */
                if (pCmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_OpenGL_SetupRenderState(drawData, iWidth, iHeight, uiVao);
                else
                    pCmd->UserCallback(cmdList, pCmd);
            }
            else
            {
                constexpr GLenum valType = sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
                
                // project scissor/clipping rectangles into framebuffer space
                Dewpsi::PointRectf clipRect;
                clipRect.x1 = (pCmd->ClipRect.x - clipOff.x) * clipScale.x;
                clipRect.y1 = (pCmd->ClipRect.y - clipOff.y) * clipScale.y;
                clipRect.x2 = (pCmd->ClipRect.z - clipOff.x) * clipScale.x;
                clipRect.y2 = (pCmd->ClipRect.w - clipOff.y) * clipScale.y;
                
                if (clipRect.x1 < iWidth && clipRect.y1 < iHeight && clipRect.x2 >= 0.0f && clipRect.y2 >= 0.0f)
                {
                    // apply scissor/clipping rectangle
                    glScissor((int) clipRect.x1, (int)(iHeight - clipRect.y2), (int)(clipRect.x2 - clipRect.x1), (int)(clipRect.y2 - clipRect.y1));
                    
                    // draw texture
                    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>((intptr_t) pCmd->TextureId));
                    glDrawElements(GL_TRIANGLES, (GLsizei) pCmd->ElemCount, valType,
                        (void*) static_cast<intptr_t>(pCmd->IdxOffset * sizeof(ImDrawIdx)));
                }
            }
        } // end for
    }
    
    // destroy the temporary VAO
    glDeleteVertexArrays(1, &uiVao);
    
    // restore the original GL state
    Dewpsi::OpenGL_RestoreState(&state);
}

bool ImGui_OpenGL_CreateFontsTexture()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    PDuchar* ucpPixels = nullptr;
    int iWidth = 0, iHeight = 0;
    
    // if your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory
    io.Fonts->GetTexDataAsRGBA32(&ucpPixels, &iWidth, &iHeight); // load as RGBA 32 bit
    
    // unload texture
    int iLastTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &iLastTexture);
    
    // new font texture
    glGenTextures(1, &g_Data.fontTexture);
    glBindTexture(GL_TEXTURE_2D, g_Data.fontTexture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_UNPACK_ROW_LENGTH
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ucpPixels);

    // store our identifier
    io.Fonts->TexID = (ImTextureID) (intptr_t) g_Data.fontTexture;

    // restore state
    glBindTexture(GL_TEXTURE_2D, iLastTexture);

    return true;
}

void ImGui_OpenGL_DestroyFontsTexture()
{
    if (g_Data.fontTexture)
    {
        ImGuiIO& io = ImGui::GetIO();
        glDeleteTextures(1, &g_Data.fontTexture);
        io.Fonts->TexID = 0;
        g_Data.fontTexture = 0;
    }
}

bool ImGui_OpenGL_CreateDeviceObjects()
{
    int iLastTex, iLastVertBuffer;
    
    // remove the area past the final slash
    std::string sFile = std::string(g_ShaderPath.get()) + "/Dewpsi_ImGui_Shaders_";
    
    // Backup GL state
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &iLastTex);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &iLastVertBuffer);
    
    // Parse GLSL version string
    int iGlslVersion = 130;
    std::sscanf(g_Data.glslVersionString, "#version %d", &iGlslVersion);
    
    // file based on GLSL version
    if (iGlslVersion < 130)
        sFile += "v120.glsl";
    else if (iGlslVersion >= 410)
        sFile += "v410.glsl";
    else if (iGlslVersion == 300)
        sFile += "v300.glsl";
    else
        sFile += "v130.glsl";
    
    // compile shaders
    ShaderProgramSource sources = ParseShaderFile(sFile);
    g_Data.shader = CreateShader(sources);
    
    // uniforms
    g_Data.attribLocationTex = glGetUniformLocation(g_Data.shader, "Texture");
    g_Data.attribLocationProjMtx = glGetUniformLocation(g_Data.shader, "ProjMtx");
    
    // vertex attributes
    g_Data.attribLocationVtxPos = glGetAttribLocation(g_Data.shader, "Position");
    g_Data.attribLocationVtxUV = (PDuint)glGetAttribLocation(g_Data.shader, "UV");
    g_Data.attribLocationVtxColor = (PDuint)glGetAttribLocation(g_Data.shader, "Color");
    
    // Create buffers
    glGenBuffers(1, &g_Data.vboHandle);
    glGenBuffers(1, &g_Data.elementsHandle);
    
    ImGui_OpenGL_CreateFontsTexture();
    
    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, iLastTex);
    glBindBuffer(GL_ARRAY_BUFFER, iLastVertBuffer);
    
    return true;
}

void ImGui_OpenGL_DestroyDeviceObjects()
{
    if (g_Data.vboHandle)
    {
        glDeleteBuffers(1, &g_Data.vboHandle);
        g_Data.vboHandle = 0;
    }
    
    if (g_Data.elementsHandle)
    {
        glDeleteBuffers(1, &g_Data.elementsHandle);
        g_Data.elementsHandle = 0;
    }
    
    // delete shaders
    Dewpsi_ImGui_Impl::DeleteShaders(g_Data.vertShader, g_Data.fragShader);
    glDeleteProgram(g_Data.shader);
    g_Data.shader = 0;
//    g_Data.vertShader = g_Data.fragShader = g_Data.shader = 0;
    
    if (g_Data.shader && g_Data.vertShader)
        glDetachShader(g_Data.shader, g_Data.vertShader);
    
    if (g_Data.shader && g_Data.fragShader)
        glDetachShader(g_Data.shader, g_Data.fragShader);
    
    g_Data.vertShader = g_Data.fragShader = 0;
    
    ImGui_OpenGL_DestroyFontsTexture();
}
