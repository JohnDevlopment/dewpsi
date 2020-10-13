#include "Dewpsi_Math.h"

namespace Dewpsi {

PDlong pow2(PDsizei n, const PDsizei sizeOfType)
{
    long int result = 1;
    const PDsizei nn = n;

    while (n > 1)
    {
        result <<= 1;
        if (result == 1)
        {
            char temp[50];
            std::sprintf(temp, "%lu is too large an argument for this type", (long) nn);
            throw std::overflow_error(temp);
        }
        --n;
    }

    return n ? result : 0;
}

}
