#include <iostream>

#include "include/Dewpsi_Core.h"

using namespace std;

template <typename T>
struct ToHex {
    T val;
};

template <typename T>
ToHex<T> MakeToHex(T val)
{
    ToHex<T> ret;
    ret.val = val;
    return ret;
}

template <typename T>
ostream& operator<<(ostream& os, const ToHex<T>& hex)
{
    os << "0x" << std::hex << (long int) hex.val << std::dec;
    return os;
}

int main(int argc, const char* argv[])
{
    
}
