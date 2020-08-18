#ifndef DEWPSI_SDLPLATFORM_H
#define DEWPSI_SDLPLATFORM_H

/** @file  Dewpsi_SDLPlatform.h
*   @brief @doxfb
*   SDL platform header.
*/

#include <Dewpsi_Core.h>

namespace Dewpsi {
    /// ImGui init data for SDL platforms.
    /// @ingroup sdl
    struct ImGuiInitData {
        std::string glslPath;
        std::string glslVersion;

        ImGuiInitData() : glslPath(), glslVersion()
        {  }
    };
}

#endif /* DEWPSI_SDLPLATFORM_H */
