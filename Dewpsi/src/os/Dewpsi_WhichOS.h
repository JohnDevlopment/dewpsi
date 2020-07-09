#ifndef DEWPSI_WHICHOS_H
#define DEWPSI_WHICHOS_H

#if defined(PD_PLATFORM_LINUX) || defined(__linux__)
    #include <Dewpsi_SDLWindow.h>
#else
    #error Currently only Linux is supported
#endif

#endif /* DEWPSI_WHICHOS_H */
