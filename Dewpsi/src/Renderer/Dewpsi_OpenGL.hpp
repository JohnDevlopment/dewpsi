#ifndef DEWPSI_OPENGL_H
#define DEWPSI_OPENGL_H

#include <Dewpsi_Core.h>
//#include <Dewpsi_Application.h>

namespace Dewpsi_ImGui_Impl {
    // OpenGL data structure
    struct OpenGLData {
        PDuint glVersion;            // Extracted at runtime using GL_MAJOR_VERSION, GL_MINOR_VERSION queries (e.g. 320 for GL 3.2)
        PDuint fontTexture;
        PDuint shader;
        PDuint vertShader;
        PDuint fragShader;
        int attribLocationTex;       // Uniforms location
        int attribLocationProjMtx;   //
        PDuint attribLocationVtxPos;
        PDuint attribLocationVtxUV;
        PDuint attribLocationVtxColor;
        PDuint vboHandle; // vertex buffer object
        PDuint elementsHandle;
        char glslVersionString[32];  // Specified by user or detected based on compile time GL settings.
    };

    // OpenGL state structure
    struct OpenGLState {
        PDuint lastActiveTexture;
        PDuint program;
        PDuint texture;
#ifdef GL_SAMPLER_BINDING
        PDuint lastSampler;
#endif
        PDuint lastArrayBuffer;
#ifdef GL_POLYGON_MODE
        PDuint lastPolygonMode[2];
#endif
        int viewport[4];
        int scissorBox[4];
        PDuint lastSourceRGB;
        PDuint lastDestRGB;
        PDuint lastSourceAlpha;
        PDuint lastDestAlpha;
        PDuint lastBlendEqRGB;
        PDuint lastBlendEqAlpha;
        bool isBlend;
        bool isCullFace;
        bool isDepthTest;
        bool isScissorTest;
    };





    ShaderProgramSource ParseShaderFile(const std::string& filename);
    void ResizeErrorPtr(unsigned int len);
    bool CheckProgram(unsigned int program, const char* desc);
    bool CheckShader(unsigned int shader, const char* desc);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const ShaderProgramSource& source, PDuint* pVert = nullptr, PDuint* pFrag = nullptr);
}

namespace Dewpsi {
    using Dewpsi_ImGui_Impl::OpenGLState;

    /// @addtogroup sdl
    /// @{

    /** Initialize OpenGL shaders.
    *   @param  shaderFile  A string containing the path to a shader file
    *   @return             True if successful or false on failure
    */
    PD_CALL bool OpenGL_InitShaders(const char* shaderFile);

    /** Frees all memory associated with the shaders created through OpenGL_InitShaders().
    *   Should be called when shutting down the program provided OpenGL_InitShaders() was called.
    */
    PD_CALL void OpenGL_DeInitShaders();

    /// Returns the shader program.
    PD_CALL PDuint OpenGL_GetShader();

    /// Backup the GL state.
    PD_CALL void OpenGL_BackupState(OpenGLState* state);

    /// Restore the GL state.
    PD_CALL void OpenGL_RestoreState(const OpenGLState* state);

    /// @}
}



#endif /* DEWPSI_OPENGL_H */
