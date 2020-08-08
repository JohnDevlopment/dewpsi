#ifndef DEWPSI_PLATFORM_H
#define DEWPSI_PLATFORM_H

/**
*   @file       Dewpsi_Platform.h
*   @brief      @doxfb
*   Platform-independent functions.
*
*   @defgroup platform Platform-Independent Functions
*   Platform-independent functions.
*   @ingroup core
*/

/**
*   @page page_platform Platform-Specific Instructions On Using ImGui
*   @tableofcontents
*   @section Initialization
*   This section details the steps leading up to the call to InitImGui(), which
*   is called automatically by the ImGuiLayer.
*
*   @par Linux
*   Use the structure @c ImGuiInitData to define the parameters for InitImGui().
*   The definition of @c ImGuiInitData is as follows:
*   @snippet structs.cpp ImGuiInitData
*
*   @par
*   The data shown above is used to initialize ImGui with the correct OpenGL
*   renderer and its associated shader language. @a glslPath is a string containing
*   the path to the directory that contains the shader files used by ImGui. The path can be absolute or
*   relative to the current working directory.
*   @a glslVersion is a string of the form "#version xxx [core|es]", which tells ImGui which version
*   of GLSL to use. @a xxx refers to a three-digit version number of GLSL. @a core and @a es are specific
*   to certain versions of GLSL. For a reference of what GLSL version corresponds to your OpenGL version,
*   check the table below. The column labelled "GLSL Version String" is the string you give to @a glslVersion.
*
*   OpenGL | GLSL Version | GLSL Version String
    ------ | ------------ | -------------------
    2.0    | 110          | "#version 110"
    2.1    | 120          | "#version 120"
    3.0    | 130          | "#version 130"
    3.1    | 140          | "#version 140"
    3.2    | 150          | "#version 150"
    3.3    | 330          | "#version 330 core"
    4.0    | 400          | "#version 400 core"
    4.1    | 400          | "#version 410 core"
    4.2    | 400          | "#version 420 core"
    4.3    | 400          | "#version 430 core"
    ES 2.0 | 100          | "#version 100"      = WebGL 1.0
    ES 3.0 | 300          | "#version 300 es"   = WebGL 2.0
*
*   @par
*   Here is an example of how to use this table:
*   @code{.cpp}
    // OpenGL version 3.2
    Dewpsi::Platform::ImGuiInitData data;
    data.glslPath = "directory/shaders.glsl";
    data.glslVersion = "#version 150";
    Dewpsi::Platform::InitImGui(&data);
*   @endcode
*
*   @par
*   One of four predefined shader files is read according to the GLSL version
*   provided. The file is located in the directory provided in @a glslPath.
*   1. If less than 130, Dewpsi_ImGui_Shaders_v120.glsl
*   2. If 410 or greater, Dewpsi_ImGui_Shaders_v410.glsl
*   3. If 300, Dewpsi_ImGui_Shaders_v300.glsl
*   4. If 1-3 are missed, Dewpsi_ImGui_Shaders_v130.glsl is the fallback
*/

#include <Dewpsi_Core.h>
#include <Dewpsi_WhichOS.h>

namespace Dewpsi {
    namespace Platform {
        /**
        *   @addtogroup platform
        *   @{
        */
        
        /** Obtains the current process time.
        *   @return The number of seconds since the program started in seconds
        */
        PD_CALL PDfloat GetTime();
        
        /** Initializes ImGui with the data found in @a data.
        *   @param  data    A pointer to platform-specific data
        *   @return         Upon success, PD_SUCCESS; upon failure, PD_FAILURE or PD_INVALID
        *
        *   @note           On Linux platforms, @a data is a pointer to @c ImGuiInitData.
        *   @par            ImGuiInitData
        *   @snippet        structs.cpp ImGuiInitData
        */
        PD_CALL PDenum InitImGui(const void* data);
        
        /** Shuts down ImGui.
        *   After this is called, ImGui functions can no longer be used.
        */
        PD_CALL void QuitImGui();
        
        /** Returns a pointer to a platform-specific structure.
        *   On Linux platforms that type is @c ImGuiInitData.
        *   @see
        *   @c InitImGui for Linux-specific data structure.
        */
        PD_CALL void* GetImGuiInitData();
        
        /// @}
    }
}

#endif /* DEWPSI_PLATFORM_H */
