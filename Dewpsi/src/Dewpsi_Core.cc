#include "Dewpsi_Core.h"

namespace Dewpsi {

int GetOption(int argc, char** argv, const char* optstring)
{
    return my_getopt(argc, argv, optstring);
}

}
