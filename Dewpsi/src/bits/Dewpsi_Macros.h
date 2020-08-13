#ifndef DEWPSI_MACROS_H
#define DEWPSI_MACROS_H

#if defined(__GNUC__)

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

#ifndef PD_MEMSET
    #ifdef __has_builtin
        #if __has_builtin(memset)
            #define PD_MEMSET(d,c,l)    __builtin_memset(d,c,l)
        #else
            #define PD_MEMSET(d,c,l)    ::std::memset(d,c,l)
        #endif
    #else
        #define PD_MEMSET(d,c,l)        ::std::memset(d,c,l)
    #endif
#endif

#ifndef PD_SSCANF
    #ifdef __has_builtin
        #if __has_builtin(sscanf)
            #define PD_SSCANF(s,f,...)  __builtin_sscanf(s,f,__VA_ARGS__)
        #else
            #define PD_SSCANF(s,f,...)  ::std::sscanf(s,f,__VA_ARGS__)
        #endif
    #else
        #define PD_SSCANF(s,f,...)      ::std::sscanf(s,f,__VA_ARGS__)
    #endif
#endif

#endif /* defined(__GNUC__) */

#endif /* DEWPSI_MACROS_H */
