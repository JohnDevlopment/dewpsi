#ifndef BEGIN_CODE_H
#define BEGIN_CODE_H

// define GETOPT_ATTR
#ifdef __GNUC__
    #define GETOPT_ATTR(...) __attribute__((__VA_ARGS__))
#elif defined(_MSC_VER)
    #define GETOPT_ATTR(...) __declspec(__VA_ARGS__)
#endif

#define GETOPT_HIDDEN hidden
#define GETOPT_NOTHROW nothrow
#define GETOPT_LEAF leaf
#define GETOPT_NONNULLP(...) nonnull(__VA_ARGS__)

#define GETOPT_NONNULL nonnull

#endif /* BEGIN_CODE_H */
