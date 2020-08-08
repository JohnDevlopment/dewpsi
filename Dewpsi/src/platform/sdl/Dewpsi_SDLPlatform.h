#ifndef DEWPSI_SDLPLATFORM_H
#define DEWPSI_SDLPLATFORM_H

#include <Dewpsi_Core.h>

namespace Dewpsi {
    struct ImGuiInitData {
        std::string glslPath;
        std::string glslVersion;
        
        ImGuiInitData() : glslPath(), glslVersion()
        {  }
    };
}

#endif /* DEWPSI_SDLPLATFORM_H */
