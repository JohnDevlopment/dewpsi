#ifndef BEGIN_MACROS_H
#define BEGIN_MACROS_H

#ifndef _
# ifndef __GNUC__
#  define _(s) (s)
# endif
#endif

#ifdef __GNUC__
# define STRCHR(s, c) __builtin_strchr(s, c)
#else
# define STRCHR(s, c) strchr(s, c)
#endif

#ifdef __GNUC__
# define STRRCHR(s, c) __builtin_strrchr(s, c)
#else
# define STRRCHR(s, c) strrchr(s, c)
#endif

#ifdef __GNUC__
# define STRCMP(s, c) __builtin_strcmp(s, c)
#else
# define STRCMP(s, c) strcmp(s, c)
#endif

#ifdef __GNUC__
# define eprintf(fmt, ...) __builtin_fprintf(stderr, fmt, __VA_ARGS__)
#else
# define eprintf(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#endif

#endif /* BEGIN_MACROS_H */
