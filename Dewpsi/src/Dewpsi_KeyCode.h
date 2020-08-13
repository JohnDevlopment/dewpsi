#ifndef DEWPSI_KEYCODE_H
#define DEWPSI_KEYCODE_H

#include <Dewpsi_Core.h>

/**
*   @file       Dewpsi_KeyCode.h
*   @brief      @doxfb
*   Contains key codes.
*   @ingroup    keyevents
*/

namespace Dewpsi {
    /// @addtogroup keyevents
    /// @{

    /** Virtual key modifier.
    *   @ingroup keyevents
    */
    enum KeyMod : uint16_t {
        None = 0, ///< No modifier

        LeftShift    = BIT(0),  ///< Left Shift
        RightShift   = BIT(1),  ///< Right Shift
        LeftControl  = BIT(2),  ///< Left Control
        RightControl = BIT(3),  ///< Right Control
        LeftAlt      = BIT(4),  ///< Left Alt
        RightAlt     = BIT(5),  ///< Right Alt
        LeftGui      = BIT(6),  ///< Left Gui
        RightGui     = BIT(7),  ///< Right Gui
        NumLock      = BIT(8),  ///< Num lock
        CapsLock     = BIT(9),  ///< Caps lock
        Mode         = BIT(10), ///< Mode

        Shift        = LeftShift   | RightShift,   ///< Shift
        Control      = LeftControl | RightControl, ///< Control
        Alt          = LeftAlt     | RightAlt,     ///< Alt
        Gui          = LeftGui     | RightGui      ///< Gui
    };

    /** Virtual keycode.
    *   A value of this type can be passed to cout.
    */
    typedef enum class KeyCode : uint16_t {
        Unknown             = 0, ///< Unknown key

        // From glfw3.h
        Space               = 32, ///< Spacebar
        Exclaim             = 33, ///< Exclaimation point (!)
        DoubleQuote         = 34, ///< Double quote(")
        Hash                = 35, ///< Hash/pound sign (#)
        Dollar              = 36, ///< Dollar sign ($)
        Percent             = 37, ///< Percent (%)
        Ampersand           = 38, ///< Ampersand
        Apostrophe          = 39, ///< Apostrophe key (')
        LeftParen           = 40, ///< Left parenthesis
        RightParen          = 41, ///< Right parenthesis
        Asterik             = 42, ///< Asterik (*)
        Plus                = 43, ///< Plus sign (+)
        Comma               = 44, ///< Comma key (,)
        Minus               = 45, ///< Minus key (-)
        Period              = 46, ///< Period key (.)
        Slash               = 47, ///< Forward slash key (/)

        D0                  = 48, ///< Number pad 0
        D1                  = 49, ///< Number pad 1
        D2                  = 50, ///< Number pad 2
        D3                  = 51, ///< Number pad 3
        D4                  = 52, ///< Number pad 4
        D5                  = 53, ///< Number pad 5
        D6                  = 54, ///< Number pad 6
        D7                  = 55, ///< Number pad 7
        D8                  = 56, ///< Number pad 8
        D9                  = 57, ///< Number pad 9

        Colon               = 58, ///< Colon (:)
        Semicolon           = 59, ///< Semicolon (;)
        Less                = 60, ///< Less than (<)
        Equal               = 61, ///< Number pad equal (=)
        Greater             = 62, ///< Greater than (>)
        Question            = 63, ///< Question mark (?)
        At                  = 64, ///< At (@)

        A                   = 65, ///< A
        B                   = 66, ///< B
        C                   = 67, ///< C
        D                   = 68, ///< D
        E                   = 69, ///< E
        F                   = 70, ///< F
        G                   = 71, ///< G
        H                   = 72, ///< H
        I                   = 73, ///< I
        J                   = 74, ///< J
        K                   = 75, ///< K
        L                   = 76, ///< L
        M                   = 77, ///< M
        N                   = 78, ///< N
        O                   = 79, ///< O
        P                   = 80, ///< P
        Q                   = 81, ///< Q
        R                   = 82, ///< R
        S                   = 83, ///< S
        T                   = 84, ///< T
        U                   = 85, ///< U
        V                   = 86, ///< V
        W                   = 87, ///< W
        X                   = 88, ///< X
        Y                   = 89, ///< Y
        Z                   = 90, ///< Z

        LeftBracket         = 91, ///< Left bracket ([)
        Backslash           = 92, ///< Backslash (\)
        RightBracket        = 93, ///< Right Bracket (])
        Caret               = 94, ///< Caret (^)
        Underscore          = 95, ///< Underscore (_)
        GraveAccent         = 96, ///< Grave accent (`)

        LeftBrace           = 123, ///< Left brace({)
        Pipe                = 124, ///< Pipe (|)
        RightBrace          = 125, ///< Right brace (})
        Tilde               = 126, ///< Tilde (~)

        World1              = 161, ///< Non-US #1
        World2              = 162, ///< Non-US #2

        /* Function keys */
        Escape              = 256, ///< Escape key
        Enter               = 257, ///< Enter/Return key
        Tab                 = 258, ///< Tab key
        Backspace           = 259, ///< Backspace key
        Insert              = 260, ///< Insert key
        Delete              = 261, ///< Delete key
        Right               = 262, ///< Right arrow key
        Left                = 263, ///< Left arrow key
        Down                = 264, ///< Down arrow key
        Up                  = 265, ///< Up arrow key
        PageUp              = 266, ///< Page up
        PageDown            = 267, ///< Page down
        Home                = 268, ///< Home
        End                 = 269, ///< End
        CapsLock            = 280, ///< Caps lock
        ScrollLock          = 281, ///< Scroll lock
        NumLock             = 282, ///< Num lock
        PrintScreen         = 283, ///< Print screen
        Pause               = 284, ///< Pause
        F1                  = 290, ///< Function key 1
        F2                  = 291, ///< Function key 2
        F3                  = 292, ///< Function key 3
        F4                  = 293, ///< Function key 4
        F5                  = 294, ///< Function key 5
        F6                  = 295, ///< Function key 6
        F7                  = 296, ///< Function key 7
        F8                  = 297, ///< Function key 8
        F9                  = 298, ///< Function key 9
        F10                 = 299, ///< Function key 10
        F11                 = 300, ///< Function key 11
        F12                 = 301, ///< Function key 12

        /* Keypad */
        KP0                 = 320, ///< Keypad 0
        KP1                 = 321, ///< Keypad 1
        KP2                 = 322, ///< Keypad 2
        KP3                 = 323, ///< Keypad 3
        KP4                 = 324, ///< Keypad 4
        KP5                 = 325, ///< Keypad 5
        KP6                 = 326, ///< Keypad 6
        KP7                 = 327, ///< Keypad 7
        KP8                 = 328, ///< Keypad 8
        KP9                 = 329, ///< Keypad 9

        KPPeriod            = 330, ///< Keypad decimal
        KPDivide            = 331, ///< Keypad divide (/)
        KPMultiply          = 332, ///< Keypad multiply (*)
        KPSubtract          = 333, ///< Keypad subtract (-)
        KPAdd               = 334, ///< Keypad add (+)
        KPEnter             = 335, ///< Keypad Enter
        KPEqual             = 336, ///< Keypad equal (=)

        Application         = 337, ///< Application/Window button

        LeftShift           = 340, ///< Left shift
        LeftControl         = 341, ///< Left control
        LeftAlt             = 342, ///< Left alt
        LeftSuper           = 343, ///< Left super
        RightShift          = 344, ///< Right shift
        RightControl        = 345, ///< Right control
        RightAlt            = 346, ///< Right alt
        RightSuper          = 347, ///< Right super
        LeftMenu            = 348, ///< Left menu/gui key
        RightMenu           = 349, ///< Right menu/gui key

        Count               = 132, ///< Number of key codes
    } Key;

    /// Prints the value of a keycode as an integer.
    inline std::ostream& operator<<(std::ostream& os, KeyCode code)
    {
        os << static_cast<int32_t>(code);
        return os;
    }

    /// @}
}

/// @addtogroup keyevents
/// @{

/// Unknown key
#define PD_KEY_UNKNOWN          ::Dewpsi::Key::Unknown
/// Spacebar
#define PD_KEY_SPACE            ::Dewpsi::Key::Space
/// Exclaimation point (!)
#define PD_KEY_EXCLAIM          ::Dewpsi::Key::Exclaim
/// Double quote(")
#define PD_KEY_DOUBLEQUOTE      ::Dewpsi::Key::DoubleQuote
/// Hash/pound sign (#)
#define PD_KEY_HASH             ::Dewpsi::Key::Hash
/// Dollar sign ($)
#define PD_KEY_DOLLAR           ::Dewpsi::Key::Dollar
/// Percent (%)
#define PD_KEY_PERCENT          ::Dewpsi::Key::Percent
/// Ampersand (&)
#define PD_KEY_AMPERSAND        ::Dewpsi::Key::Ampersand
/// Apostrophe key (')
#define PD_KEY_APOSTROPHE       ::Dewpsi::Key::Apostrophe
/// Left parenthesis
#define PD_KEY_LEFTPAREN        ::Dewpsi::Key::LeftParen
/// Right parenthesis
#define PD_KEY_RIGHTPAREN       ::Dewpsi::Key::RightParen
/// Asterik (*)
#define PD_KEY_ASTERISK         ::Dewpsi::Key::Asterik
/// Plus sign (+)
#define PD_KEY_PLUS             ::Dewpsi::Key::Plus
/// Comma
#define PD_KEY_COMMA            ::Dewpsi::Key::Comma
/// Minus key
#define PD_KEY_MINUS            ::Dewpsi::Key::Minus
/// Period
#define PD_KEY_PERIOD           ::Dewpsi::Key::Period
/// Forward slash
#define PD_KEY_SLASH            ::Dewpsi::Key::Slash
/// Numpad 0
#define PD_KEY_0                ::Dewpsi::Key::D0
/// Numpad 1
#define PD_KEY_1                ::Dewpsi::Key::D1
/// Numpad 2
#define PD_KEY_2                ::Dewpsi::Key::D2
/// Numpad 3
#define PD_KEY_3                ::Dewpsi::Key::D3
/// Numpad 4
#define PD_KEY_4                ::Dewpsi::Key::D4
/// Numpad 5
#define PD_KEY_5                ::Dewpsi::Key::D5
/// Numpad 6
#define PD_KEY_6                ::Dewpsi::Key::D6
/// Numpad 7
#define PD_KEY_7                ::Dewpsi::Key::D7
/// Numpad 8
#define PD_KEY_8                ::Dewpsi::Key::D8
/// Numpad 9
#define PD_KEY_9                ::Dewpsi::Key::D9
/// Colon (:)
#define PD_KEY_COLON            ::Dewpsi::Key::Colon
/// Semicolon (;)
#define PD_KEY_SEMICOLON        ::Dewpsi::Key::Semicolon
/// Less-than symbol (<)
#define PD_KEY_LESS             ::Dewpsi::Key::Less
/// Equal (=)
#define PD_KEY_EQUAL            ::Dewpsi::Key::Equal
/// Greater than (>)
#define PD_KEY_GREATER          ::Dewpsi::Key::Greater
/// Question mark (?)
#define PD_KEY_QUESTION         ::Dewpsi::Key::Question
/// At (@)
#define PD_KEY_AT               ::Dewpsi::Key::At
/// A
#define PD_KEY_A                ::Dewpsi::Key::A
/// B
#define PD_KEY_B                ::Dewpsi::Key::B
/// C
#define PD_KEY_C                ::Dewpsi::Key::C
/// D
#define PD_KEY_D                ::Dewpsi::Key::D
/// E
#define PD_KEY_E                ::Dewpsi::Key::E
/// F
#define PD_KEY_F                ::Dewpsi::Key::F
/// G
#define PD_KEY_G                ::Dewpsi::Key::G
/// H
#define PD_KEY_H                ::Dewpsi::Key::H
/// I
#define PD_KEY_I                ::Dewpsi::Key::I
/// J
#define PD_KEY_J                ::Dewpsi::Key::J
/// K
#define PD_KEY_K                ::Dewpsi::Key::K
/// L
#define PD_KEY_L                ::Dewpsi::Key::L
/// M
#define PD_KEY_M                ::Dewpsi::Key::M
/// N
#define PD_KEY_N                ::Dewpsi::Key::N
/// O
#define PD_KEY_O                ::Dewpsi::Key::O
/// P
#define PD_KEY_P                ::Dewpsi::Key::P
/// Q
#define PD_KEY_Q                ::Dewpsi::Key::Q
/// R
#define PD_KEY_R                ::Dewpsi::Key::R
/// S
#define PD_KEY_S                ::Dewpsi::Key::S
/// T
#define PD_KEY_T                ::Dewpsi::Key::T
/// U
#define PD_KEY_U                ::Dewpsi::Key::U
/// V
#define PD_KEY_V                ::Dewpsi::Key::V
/// W
#define PD_KEY_W                ::Dewpsi::Key::W
/// X
#define PD_KEY_X                ::Dewpsi::Key::X
/// Y
#define PD_KEY_Y                ::Dewpsi::Key::Y
/// Z
#define PD_KEY_Z                ::Dewpsi::Key::Z
/// Left bracket
#define PD_KEY_LEFTBRACKET      ::Dewpsi::Key::LeftBracket
/// Backslash
#define PD_KEY_BACKSLASH        ::Dewpsi::Key::Backslash
/// Right bracket
#define PD_KEY_RIGHTBRACKET     ::Dewpsi::Key::RightBracket
/// Caret (^)
#define PD_KEY_CARET            ::Dewpsi::Key::Caret
/// Underscore (_)
#define PD_KEY_UNDERSCORE       ::Dewpsi::Key::Underscore
/// Grave accent (`)
#define PD_KEY_GRAVEACCENT      ::Dewpsi::Key::GraveAccent
/// Left brace({)
#define PD_KEY_LEFTBRACE        ::Dewpsi::Key::LeftBrace
/// Pipe (|)
#define PD_KEY_PIPE             ::Dewpsi::Key::Pipe
/// Right brace (})
#define PD_KEY_RIGHTBRACE       ::Dewpsi::Key::RightBrace
/// Tilde (~)
#define PD_KEY_TILDE            ::Dewpsi::Key::Tilde
/// Non-US #1
#define PD_KEY_WORLD1           ::Dewpsi::Key::World1
/// Non-US #2
#define PD_KEY_WORLD2           ::Dewpsi::Key::World2

/// Escape
#define PD_KEY_ESCAPE           ::Dewpsi::Key::Escape
/// Enter/Return
#define PD_KEY_ENTER            ::Dewpsi::Key::Enter
/// Tab
#define PD_KEY_TAB              ::Dewpsi::Key::Tab
/// Backspace
#define PD_KEY_BACKSPACE        ::Dewpsi::Key::Backspace
/// Insert
#define PD_KEY_INSERT           ::Dewpsi::Key::Insert
/// Delete
#define PD_KEY_DELETE           ::Dewpsi::Key::Delete
/// Right arrow key
#define PD_KEY_RIGHT            ::Dewpsi::Key::Right
/// Left arrow key
#define PD_KEY_LEFT             ::Dewpsi::Key::Left
/// Down arrow key
#define PD_KEY_DOWN             ::Dewpsi::Key::Down
/// Up arrow key
#define PD_KEY_UP               ::Dewpsi::Key::Up
/// Page up
#define PD_KEY_PAGEUP           ::Dewpsi::Key::PageUp
/// Page down
#define PD_KEY_PAGEDOWN         ::Dewpsi::Key::PageDown
/// Home key
#define PD_KEY_HOME             ::Dewpsi::Key::Home
/// End key
#define PD_KEY_END              ::Dewpsi::Key::End
/// Caps lock
#define PD_KEY_CAPSLOCK         ::Dewpsi::Key::CapsLock
/// Scroll lock
#define PD_KEY_SCROLLLOCK       ::Dewpsi::Key::ScrollLock
/// Num lock
#define PD_KEY_NUMLOCK          ::Dewpsi::Key::NumLock
/// Print screen
#define PD_KEY_PRINTSCREEN      ::Dewpsi::Key::PrintScreen
/// Pause button
#define PD_KEY_PAUSE            ::Dewpsi::Key::Pause

/// Function key 1
#define PD_KEY_F1               ::Dewpsi::Key::F1
/// Function key 1
#define PD_KEY_F2               ::Dewpsi::Key::F2
/// Function key 1
#define PD_KEY_F3               ::Dewpsi::Key::F3
/// Function key 1
#define PD_KEY_F4               ::Dewpsi::Key::F4
/// Function key 1
#define PD_KEY_F5               ::Dewpsi::Key::F5
/// Function key 1
#define PD_KEY_F6               ::Dewpsi::Key::F6
/// Function key 1
#define PD_KEY_F7               ::Dewpsi::Key::F7
/// Function key 1
#define PD_KEY_F8               ::Dewpsi::Key::F8
/// Function key 1
#define PD_KEY_F9               ::Dewpsi::Key::F9
/// Function key 1
#define PD_KEY_F10              ::Dewpsi::Key::F10
/// Function key 1
#define PD_KEY_F11              ::Dewpsi::Key::F11
/// Function key 1
#define PD_KEY_F12              ::Dewpsi::Key::F12
/// Function key 1
#define PD_KEY_F13              ::Dewpsi::Key::F13
/// Function key 1
#define PD_KEY_F14              ::Dewpsi::Key::F14
/// Function key 1
#define PD_KEY_F15              ::Dewpsi::Key::F15
/// Function key 1
#define PD_KEY_F16              ::Dewpsi::Key::F16
/// Function key 1
#define PD_KEY_F17              ::Dewpsi::Key::F17
/// Function key 1
#define PD_KEY_F18              ::Dewpsi::Key::F18
/// Function key 1
#define PD_KEY_F19              ::Dewpsi::Key::F19
/// Function key 1
#define PD_KEY_F20              ::Dewpsi::Key::F20
/// Function key 1
#define PD_KEY_F21              ::Dewpsi::Key::F21
/// Function key 1
#define PD_KEY_F22              ::Dewpsi::Key::F22
/// Function key 1
#define PD_KEY_F23              ::Dewpsi::Key::F23
/// Function key 1
#define PD_KEY_F24              ::Dewpsi::Key::F24
/// Function key 1
#define PD_KEY_F25              ::Dewpsi::Key::F25

// keypad
/// Keypad 0
#define PD_KEY_KP0              ::Dewpsi::Key::KP0
/// Keypad 1
#define PD_KEY_KP1              ::Dewpsi::Key::KP1
/// Keypad 2
#define PD_KEY_KP2              ::Dewpsi::Key::KP2
/// Keypad 3
#define PD_KEY_KP3              ::Dewpsi::Key::KP3
/// Keypad 4
#define PD_KEY_KP4              ::Dewpsi::Key::KP4
/// Keypad 5
#define PD_KEY_KP5              ::Dewpsi::Key::KP5
/// Keypad 6
#define PD_KEY_KP6              ::Dewpsi::Key::KP6
/// Keypad 7
#define PD_KEY_KP7              ::Dewpsi::Key::KP7
/// Keypad 8
#define PD_KEY_KP8              ::Dewpsi::Key::KP8
/// Keypad 9
#define PD_KEY_KP9              ::Dewpsi::Key::KP9
/// Keypad decimal (.)
#define PD_KEY_KPPERIOD         ::Dewpsi::Key::KPPeriod
/// Keypad divide (/)
#define PD_KEY_KPDIVIDE         ::Dewpsi::Key::KPDivide
/// Keypad multiply (*)
#define PD_KEY_KPMULTIPLY       ::Dewpsi::Key::KPMultiply
/// Keypad subtract (-)
#define PD_KEY_KPSUBTRACT       ::Dewpsi::Key::KPSubtract
/// Keypad add (+)
#define PD_KEY_KPADD            ::Dewpsi::Key::KPAdd
/// Keypad Enter
#define PD_KEY_KPENTER          ::Dewpsi::Key::KPEnter
/// Keypad equal (=)
#define PD_KEY_KPEQUAL          ::Dewpsi::Key::KPEqual
/// Window/Application button
#define PD_KEY_APPLICATION      ::Dewpsi::Key::Application

/// Left shift
#define PD_KEY_LEFTSHIFT        ::Dewpsi::Key::LeftShift
/// Left control
#define PD_KEY_LEFTCONTROL      ::Dewpsi::Key::LeftControl
/// Left alt
#define PD_KEY_LEFTALT          ::Dewpsi::Key::LeftAlt
/// Left super
#define PD_KEY_LEFTSUPER        ::Dewpsi::Key::LeftSuper
/// Right shift
#define PD_KEY_RIGHTSHIFT       ::Dewpsi::Key::RightShift
/// Right control
#define PD_KEY_RIGHTCONTROL     ::Dewpsi::Key::RightControl
/// Right alt
#define PD_KEY_RIGHTALT         ::Dewpsi::Key::RightAlt
/// Right super
#define PD_KEY_RIGHTSUPER       ::Dewpsi::Key::RightSuper
/// Left window/gui key
#define PD_KEY_LEFTGUI          ::Dewpsi::Key::LeftMenu
/// Right window/gui key
#define PD_KEY_RIGHTGUI         ::Dewpsi::Key::RightMenu
/// Number of key codes
#define PD_KEY_COUNT            ::Dewpsi::Key::Count

/// None
#define PD_MOD_NONE             ::Dewpsi::None
/// Left Shift
#define PD_MOD_LEFTSHIFT        ::Dewpsi::LeftShift
/// Right Shift
#define PD_MOD_RIGHTSHIFT       ::Dewpsi::RightShift
/// Left Control
#define PD_MOD_LEFTCONTROL      ::Dewpsi::LeftControl
/// Right Control
#define PD_MOD_RIGHTCONTROL     ::Dewpsi::RightControl
/// Left Alt
#define PD_MOD_LEFTALT          ::Dewpsi::LeftAlt
/// Right Alt
#define PD_MOD_RIGHTALT         ::Dewpsi::RightAlt
/// Left Gui
#define PD_MOD_LEFTGUI          ::Dewpsi::LeftGui
/// Right Gui
#define PD_MOD_RIGHTGUI         ::Dewpsi::RightGui
/// Num Lock
#define PD_MOD_NUMLOCK          ::Dewpsi::NumLock
/// Caps Lock
#define PD_MOD_CAPSLOCK         ::Dewpsi::CapsLock
/// Both mode keys
#define PD_MOD_MODE             ::Dewpsi::Mode

/// Both/either shift keys
#define PD_MOD_SHIFT            ::Dewpsi::Shift
/// Both/either control keys
#define PD_MOD_CONTROL          ::Dewpsi::Control
/// Both/either alt keys
#define PD_MOD_ALT              ::Dewpsi::Alt
/// Both/either gui keys
#define PD_MOD_GUI              ::Dewpsi::Gui

/// @}

#endif /* DEWPSI_KEYCODE_H */
