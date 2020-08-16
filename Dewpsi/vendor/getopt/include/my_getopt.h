#ifndef MY_GETOPT_H_INCLUDED
#define MY_GETOPT_H_INCLUDED

#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

extern char* optarg;
extern int optind;
extern int opterr;
extern int optopt;

typedef struct _GetOpt_Option {
    const char* name;
    int hasArg;
    int* flag;
    int val;
} GetOpt_Option;

#define no_argument       0
#define required_argument 1
#define optional_argument 2

// my_getopt
int
my_getopt (int argc, char** argv, const char *optstring)
    GETOPT_ATTR(GETOPT_NOTHROW, GETOPT_LEAF, GETOPT_NONNULLP(2, 3));

/*
int
my_getopt_long(int argc, char** argv, const char* optstring,
               GetOpt_Option* longopts, int* longind)
    GETOPT_ATTR(GETOPT_NOTHROW, GETOPT_LEAF, GETOPT_NONNULLP(2, 3));
*/

#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */

#include "end_code.h"

#endif /* MY_GETOPT_H_INCLUDED */
