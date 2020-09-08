#ifndef QUALIFIER_HPP__DEWPSI_INCLUDED
#define QUALIFIER_HPP__DEWPSI_INCLUDED

#include "setup.hpp"

namespace dm {
    template<length_t L, typename T> struct Vec;

#if DM_HAS_TEMPLATE_ALIASES
    //template<length_t L, typename T> using tvec1 = Vec<1, T>;
    template<length_t L, typename T> using TVec2 = Vec<2, T>;
    //template<length_t L, typename T> using tvec3 = Vec<3, T>;
    //template<length_t L, typename T> using tvec4 = Vec<4, T>;
#endif
}

#endif /* QUALIFIER_HPP__DEWPSI_INCLUDED */
