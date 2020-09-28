#ifndef DEWPSIMATH_HPP
#define DEWPSIMATH_HPP

/** @ref        core
*   @file       DewpsiMath.hpp
*
*   @defgroup   core            Core features
*   @brief                      Core features of the %dm library.
*
*   @defgroup   core_vectors    Vector types
*   @ingroup    core
*   @brief      Vector types and specifications.
*   A vector can be used to represent a point on a coordinate plane, a direction, or even
*   a color. Each component has two aliases, one representing a point/direction, and one
*   representing a color. These aliases can be used interchangeably.
*
*   Direction | Color
*   --------- | -----
*   x         | r
*   y         | g
*   z         | b
*   w         | a
*
*   @par
*   Depending on the type of vector, only some of these components are available; for example,
*   a two-component vector only has @a x and @a y, but not @a z or @a w, along with their
*   counterparts.
*
*   @mainpage Dewpsi Math (DM)
*   All functions and types are documented in their respective groups:
*   - Core feature are listed under @ref core.
*   - For all vector types, see @ref core_vectors.
*/

#include "detail/setup.hpp"

#include <cmath>
#include <climits>
#include <cfloat>
#include <cassert>
#include <typeinfo>

#include "DewpsiMath_Util.hpp"
#include "DewpsiMath_Vec2.hpp"

#endif /* DEWPSIMATH_HPP */
