#ifndef MY_GETOPT_INT_H_INCLUDED
#define MY_GETOPT_INT_H_INCLUDED

typedef enum _int_order {
    REQUIRE_ORDER,
    PERMUTE,
    RETURN_IN_ORDER
} _order;

typedef struct _int_getopt_data {
    int optind;
    int opterr;
    int optopt;
    char *optarg;

    // internal
    int __init;

    /* The next character to be scanned. If this is NULL,
       it means resume the scan by advancing to the next
       ARGV-element. */
    char* __nextchar;

    _order __ordering;
    int __first_nonopt;
    int __last_nonopt;
} _getopt_data;

#define _GETOPT_DATA_INITIALIZER { 1, 1 }

#endif /* MY_GETOPT_INT_H_INCLUDED */
