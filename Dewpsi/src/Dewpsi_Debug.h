#ifndef DEBUG_H
#define DEBUG_H

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>

#ifndef PD_PROFILE
    #define PD_PROFILE 0
#endif

namespace Dewpsi {
    using FloatMicroSeconds = std::chrono::duration<double, std::micro>;
}

#if PD_PROFILE > 0
    #if (defined(__GNUC__) || defined(__GNUG__))
        #define PD_FUNCTION_SIG         __PRETTY_FUNCTION__
    #else
        #define PD_FUNCTION_SIG         "Unknown PD_FUNCTION_SIG"
    #endif
    
    #define PD_PROFILE_SCOPE(sig)
    #define PD_PROFILE_FUNCTION()   PD_PROFILE_SCOPE(PD_FUNCTION_SIG)
#else
    #define PD_FUNCTION_SIG         "Unknown PD_FUNCTION_SIG"
    #define PD_PROFILE_SCOPE(sig)
    #define PD_PROFILE_FUNCTION()
#endif

#endif /* DEBUG_H */

