#ifndef DEWPSI_MOUSECODES_H
#define DEWPSI_MOUSECODES_H

/**
*   @file   Dewpsi_MouseCode.h
*   @brief  @doxfb
*   Contains mouse codes.
*   @ingroup mouse
*/

#include <Dewpsi_Core.h>

namespace Dewpsi {
    /// Mouse code.
    /// @ingroup mouse
    typedef enum class MouseCode : uint16_t {
        Unknown      = 0xffff,   ///< Unknown button
        Button0      = 0,        ///< Button 0
        Button1      = 1,        ///< Button 1
        Button2      = 2,        ///< Button 2
        Button3      = 3,        ///< Button 3
        Button4      = 4,        ///< Button 4
        Button5      = 5,        ///< Button 5
        Button6      = 6,        ///< Button 6
        Button7      = 7,        ///< Button 7

        LastButton   = Button7,  ///< Last mouse button
        LeftButton   = Button0,  ///< Left mouse button
        RightButton  = Button1,  ///< Right mouse button
        MiddleButton = Button2   ///< Middle mouse button (AKA, scroll wheel)
    } Mouse;

    inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

/**
*   @addtogroup mouse
*   @{
*/

/// Unknown button
#define PD_MOUSEBUTTON_UNKNOWN  ::Dewpsi::Mouse::Unknown
/// Button 0
#define PD_MOUSEBUTTON_0       ::Dewpsi::Mouse::Button0
/// Button 1
#define PD_MOUSEBUTTON_1       ::Dewpsi::Mouse::Button1
/// Button 2
#define PD_MOUSEBUTTON_2       ::Dewpsi::Mouse::Button2
/// Button 3
#define PD_MOUSEBUTTON_3       ::Dewpsi::Mouse::Button3
/// Button 4
#define PD_MOUSEBUTTON_4       ::Dewpsi::Mouse::Button4
/// Button 5
#define PD_MOUSEBUTTON_5       ::Dewpsi::Mouse::Button5
/// Button 6
#define PD_MOUSEBUTTON_6       ::Dewpsi::Mouse::Button6
/// Button 7
#define PD_MOUSEBUTTON_7       ::Dewpsi::Mouse::Button7
/// Last Button
#define PD_MOUSEBUTTON_LAST    ::Dewpsi::Mouse::LastButton
/// Left Button
#define PD_MOUSEBUTTON_LEFT    ::Dewpsi::Mouse::LeftButton
/// Right Button
#define PD_MOUSEBUTTON_RIGHT   ::Dewpsi::Mouse::RightButton
/// Middle Button
#define PD_MOUSEBUTTON_MIDDLE  ::Dewpsi::Mouse::MiddleButton

/// @}

#endif /* DEWPSI_MOUSECODES_H */
