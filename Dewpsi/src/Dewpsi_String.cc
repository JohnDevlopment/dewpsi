#include "Dewpsi_String.h"
#include "Dewpsi_Math.h"

#include <cstring>

namespace Dewpsi {

namespace String {

size_t Length(const char* str)
{
    PD_CORE_ASSERT(str, "invalid parameter 'src'");
    return PD_STRLEN(str);
}

size_t Copy(char* dst, const char* src, size_t maxlen, PDenum mode)
{
    PD_CORE_ASSERT(dst && src, "invalid parameters");
    
    const size_t szSrcLen = PD_STRLEN(src);
    size_t szDestLen = 0;
    char* cpDest = dst;
    
    if (maxlen > 0)
    {
        szDestLen = Dewpsi::min(szSrcLen, maxlen-1);
        
        // allocate new array, put address in 'cpDest' and 'dst'
        if (mode == PD_ALLOC)
        {
            char** const cppDest = (char**) dst;
            char* const cpTemp = New(szDestLen);
            if (! cpTemp)
            {
                PD_NOMEMORY();
                return PD_INVALID;
            }
            *cppDest = cpTemp;
            cpDest = *cppDest;
        }
        
        PD_MEMCPY(cpDest, src, szDestLen);
        cpDest[szDestLen] = '\0';
    }
    
    return szSrcLen;
}

size_t Cat(char* dst, const char* src, size_t maxlen)
{
    PD_CORE_ASSERT(dst && src, "invalid parameters");
    
    size_t szDstLen = Length(dst);
    size_t szSrcLen = Length(src);
    
    if (szDstLen < maxlen)
        Copy(dst + szDstLen, src, maxlen - szDstLen);
    
    return szDstLen + szSrcLen;
}

char* New(const size_t cnt, char ch)
{
    char* cpResult = nullptr;
    
    if (cnt > 0)
    {
        cpResult = new (std::nothrow) char[cnt];
        if (! cpResult)
        {
            PD_NOMEMORY();
        }
        else
        {
            std::fill_n(cpResult, cnt, ch);
        }
    }
    
    return cpResult;
}

}

}
