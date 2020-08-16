#include "Dewpsi_String.h"
#include "Dewpsi_Math.h"

#include <cstring>

constexpr size_t OPTSIZE = sizeof(PDulong);

namespace Dewpsi {

namespace String {

void MemSet(void* dst, int byte, size_t len)
{
#ifdef PD_CUSTOM_MEMSET
    PDlong lDstp = (PDlong) dst;

    if (len >= 8)
    {
        PDsizei szLen;
        PDulong cccc = (PDuchar) byte;

        cccc |= cccc << 8;
        cccc |= cccc << 16;
        if (OPTSIZE > 4)
            cccc |= (cccc << 16) << 16;

        // align pointer with the size of 'cccc'
        while ((lDstp % OPTSIZE) != 0)
        {
            reinterpret_cast<PDuint8*>(lDstp)[0] = (PDuint8) byte;
            ++lDstp;
            --len;
        }

        // write 8 per iteration until less than 8 remain
        szLen = len / (OPTSIZE * 8);
        while (szLen > 0)
        {
            reinterpret_cast<>
        }
    }

    while (len >= 0)
    {
        reinterpret_cast<PDuint8*>(lDstp)[0] = static_cast<PDuint8>(byte);
        --len;
    }

    return dst;
#else
    ::std::memset(dst, byte, len);
#endif /* PD_CUSTOM_MEMSET */
}

size_t Copy(char* dst, const char* src, size_t maxlen, PDenum mode)
{
    PD_CORE_ASSERT(dst && src, "invalid parameters");

    const size_t szSrcLen = Length(src);
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

        ::std::memcpy(cpDest, src, szDestLen);
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
