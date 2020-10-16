#ifndef DEWPSI_BITS_OBJECT_H
#define DEWPSI_BITS_OBJECT_H

#include <Dewpsi_Core.h>

namespace Dewpsi {
    struct Object {
        int refCount;
        char* bytes;
        int length;
    };
}

#endif /* DEWPSI_BITS_OBJECT_H */
