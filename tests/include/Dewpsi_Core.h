#ifndef CORE_H
#define CORE_H

#include "Dewpsi_Types.h"

// I know these work
#define BIT(x)                  (1 << x)
#define PD_LOWORD(x)            ((x) & 0xffff)
#define PD_HIWORD(x)            ((x) >> 16)
#define PD_CREATEWORD(lo, hi)   (((PDuint16)(hi) << 16) | (PDuint16)(lo))

// test these
#define PD_LONYBBLE(x)          ((PDuint8)(x) & 0x0f)
#define PD_HINYBBLE(x)          ((PDuint8)(x) >> 4)
#define PD_CREATEBYTE(lo, hi)   (((PDuint8)(hi) << 4) | ((PDuint8)(lo) & 0x0f))

#endif /* CORE_H */
