#ifndef DEWPSI_MACROS_H
#define DEWPSI_MACROS_H

#ifndef PD_STRLEN
    #ifdef __has_builtin
        #if __has_builtin(strlen)
            #define PD_STRLEN(s)    __builtin_strlen(s)
        #else
            #define PD_STRLEN(s)    ::std::strlen(s)
        #endif
    #else
        #define PD_STRLEN(s)        ::std::strlen(s)
    #endif
#endif

#ifndef PD_MEMCPY
    #ifdef __has_builtin
        #if __has_builtin(memcpy)
            #define PD_MEMCPY(d,s,l)    __builtin_memcpy(d,s,l)
        #else
            #define PD_MEMCPY(d,s,l)    ::std::memcpy(d,s,l)
        #endif
    #else
        #define PD_MEMCPY(d,s,l)        ::std::memcpy(d,s,l)
    #endif
#endif

#endif /* DEWPSI_MACROS_H */
