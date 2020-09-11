#ifndef PLATFORM_H__DEWPSI_INCLUDED
#define PLATFORM_H__DEWPSI_INCLUDED

////////////////////////////////////////////////////////////////////////////////
// Platform

#define DM_PLATFORM_UNKNOWN     0x00000000
#define DM_PLATFORM_WINDOWS     0x00010000
#define DM_PLATFORM_LINUX       0x00020000
#define DM_PLATFORM_APPLE       0x00040000
#define DM_PLATFORM_ANDROID     0x00100000
#define DM_PLATFORM_CHROME_NACL 0x00200000
#define DM_PLATFORM_UNIX        0x00400000
#define DM_PLATFORM_QNXNTO      0x00800000
#define DM_PLATFORM_WINCE       0x01000000
#define DM_PLATFORM_CYGWIN      0x02000000

#ifdef DM_FORCE_PLATFORM_UNKNOWN
#   define DM_PLATFORM DM_PLATFORM_UNKNOWN
#elif defined(__CYGWIN__)
#   define DM_PLATFORM DM_PLATFORM_CYGWIN
#elif defined(__QNXNTO__)
#   define DM_PLATFORM DM_PLATFORM_QNXNTO
#elif defined(__APPLE__)
#   define DM_PLATFORM DM_PLATFORM_APPLE
#elif defined(WINCE)
#   define DM_PLATFORM DM_PLATFORM_WINCE
#elif defined(_WIN32)
#   define DM_PLATFORM DM_PLATFORM_WINDOWS
#elif defined(__native_client__)
#   define DM_PLATFORM DM_PLATFORM_CHROME_NACL
#elif defined(__ANDROID__)
#   define DM_PLATFORM DM_PLATFORM_ANDROID
#elif defined(__linux)
#   define DM_PLATFORM DM_PLATFORM_LINUX
#elif defined(__unix)
#   define DM_PLATFORM DM_PLATFORM_UNIX
#else
#   define DM_PLATFORM DM_PLATFORM_UNKNOWN
#endif

////////////////////////////////////////////////////////////////////////////////
// Compiler

// Visual C++ defines
#define DM_COMPILER_UNKNOWN         0x00000000
#define DM_COMPILER_VC              0x01000000
#define DM_COMPILER_VC12            0x01000001
#define DM_COMPILER_VC14            0x01000002
#define DM_COMPILER_VC15            0x01000003
#define DM_COMPILER_VC15_3          0x01000004
#define DM_COMPILER_VC15_5          0x01000005
#define DM_COMPILER_VC15_6          0x01000006
#define DM_COMPILER_VC15_7          0x01000007
#define DM_COMPILER_VC15_8          0x01000008
#define DM_COMPILER_VC15_9          0x01000009
#define DM_COMPILER_VC16            0x0100000A

// GCC defines
#define DM_COMPILER_GCC             0x02000000
#define DM_COMPILER_GCC46           0x020000D0
#define DM_COMPILER_GCC47           0x020000E0
#define DM_COMPILER_GCC48           0x020000F0
#define DM_COMPILER_GCC49           0x02000100
#define DM_COMPILER_GCC5            0x02000200
#define DM_COMPILER_GCC6            0x02000300
#define DM_COMPILER_GCC7            0x02000400
#define DM_COMPILER_GCC8            0x02000500

// Clang
#define DM_COMPILER_CLANG           0x20000000
#define DM_COMPILER_CLANG34         0x20000050
#define DM_COMPILER_CLANG35         0x20000060
#define DM_COMPILER_CLANG36         0x20000070
#define DM_COMPILER_CLANG37         0x20000080
#define DM_COMPILER_CLANG38         0x20000090
#define DM_COMPILER_CLANG39         0x200000A0
#define DM_COMPILER_CLANG40         0x200000B0
#define DM_COMPILER_CLANG41         0x200000C0
#define DM_COMPILER_CLANG42         0x200000D0

// Force generic compiler
#ifdef DM_FORCE_COMPILER_UNKNOWN
#   define DM_COMPILER DM_COMPILER_UNKNOWN

// G++
#elif defined(__GNUC__) || defined(__MINGW32__)
#   if __GNUC__ >= 8
#       define DM_COMPILER DM_COMPILER_GCC8
#   elif __GNUC__ >= 7
#       define DM_COMPILER DM_COMPILER_GCC7
#   elif __GNUC__ >= 6
#       define DM_COMPILER DM_COMPILER_GCC6
#   elif __GNUC__ >= 5
#       define DM_COMPILER DM_COMPILER_GCC5
#   elif __GNUC__ == 4 && __GNUC_MINOR__ >= 9
#       define DM_COMPILER DM_COMPILER_GCC49
#   elif __GNUC__ == 4 && __GNUC_MINOR__ >= 8
#       define DM_COMPILER DM_COMPILER_GCC48
#   elif __GNUC__ == 4 && __GNUC_MINOR__ >= 7
#       define DM_COMPILER DM_COMPILER_GCC47
#   elif __GNUC__ == 4 && __GNUC_MINOR__ >= 6
#       define DM_COMPILER DM_COMPILER_GCC46
#   else
#       error "DM requires GCC 4.6 or higher"
#   endif
#else
#   define DM_COMPILER DM_COMPILER_UNKNOWN
#endif // DM_COMPILER

#ifndef DM_COMPILER
#   error "DM_COMPILER undefined, your computer may not be supported by DM. Add #define DM_COMPILER 0 to ignore this message."
#endif

#endif /* PLATFORM_H__DEWPSI_INCLUDED */
