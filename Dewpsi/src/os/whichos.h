#ifndef WHICH_H
#define WHICH_H

#ifdef PD_PLATFORM_LINUX
    #include <sdlwindow.h>
#else
    #error Currently only Linux is supported
#endif

#endif /* WHICH_H */

