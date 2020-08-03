#include "Dewpsi_String.h"
#include "Dewpsi_Math.h"

#include <cstring>

namespace Dewpsi {

namespace String {

size_t Length(const char* str)
{
    if (! str)
    {
        PD_BADPARAM("str");
        return (size_t) PD_INVALID;
    }
    
    return PD_STRLEN(str);
}

size_t Copy(char* dst, const char* src, size_t maxlen, PDenum mode)
{
    if (! dst)
    {
        PD_BADPARAM("dst");
        return PD_INVALID;
    }
    
    if (! src)
    {
        PD_BADPARAM("src");
        return PD_INVALID;
    }
    
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
                PD_OUTOFMEMORY();
                return PD_INVALID;
            }
            *cppDest = cpTemp;
            cpDest = *cppDest;
            cpTemp = nullptr;
        }
        
        PD_MEMCPY(cpDest, src, szDestLen);
        cpDest[szDestLen] = '\0';
    }
    
    return szSrcLen;
}

char* New(const size_t cnt, char ch)
{
    
    
    char* const cpResult = nullptr;
    
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
