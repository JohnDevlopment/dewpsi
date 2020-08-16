#include "my_getopt.h"
#include "my_getopt_int.h"
#include "macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = '?';

static _getopt_data _Data;

static void exchange(char** argv, _getopt_data* d)
{
	int iBottom = d->__first_nonopt;
	int iMiddle = d->__last_nonopt;
	int iTop = d->optind;
	char* cpTem;

	/* Exchange the shorter segment with the far end of the longer segment. */
	while (iTop > iMiddle && iMiddle > iBottom)
	{
		if (iTop - iMiddle > iMiddle - iBottom)
		{
			/* bottom segment is the short one */
			const int iLen = iMiddle - iBottom;

			/* swap it with the top part of the top segment */
			for (int x = 0; x < iLen; ++x)
			{
				const int out_index = iTop - iLen + x;
				cpTem = argv[iBottom + x];
				argv[iBottom + x] = argv[out_index];
				argv[out_index] = cpTem;
			}

			/* exclude the bottom segment from further swapping */
			iTop -= iLen;
		}
		else
		{
			/* top segment is the short one */
			const int iLen = iTop - iMiddle;

			/* swap it with the bottom part of the bottom segment */
			for (int x = 0; x < iLen; ++x)
			{
				cpTem = argv[iBottom + x];
				argv[iBottom + x] = argv[iMiddle + x];
				argv[iMiddle + x] = argv[iBottom + x];
			}

			/* exclude the moved top segment from further swapping */
			iBottom += iLen;
		}
	}

	/* update records for the slots the non-options now occupy */
	d->__first_nonopt += d->optind - d->__last_nonopt;
	d->__last_nonopt = d->optind;
}

static const char* getopt_initialize(int argc, char** argv,
	const char* optstring, _getopt_data* d, int posixly_correct)
{
	if (d->optind == 0)
		d->optind = 1;

	d->__first_nonopt = d->__last_nonopt = d->optind;
	d->__nextchar = NULL;

	/* first character determines the ordering of options and nonoptions */
	if (optstring[0] == '-')
	{
		d->__ordering = RETURN_IN_ORDER;
		++optstring;
	}
	else if (optstring[0] == '+')
	{
		d->__ordering = REQUIRE_ORDER;
		++optstring;
	}
	else if (posixly_correct)
	{
		d->__ordering = REQUIRE_ORDER;
		++optstring;
	}
	else
		d->__ordering = PERMUTE;

	d->__init = 1;

	return optstring;
}

static int getopt_internal_r(int argc, char** argv, const char* optstring,
	const GetOpt_Option* longopts, int* longind,
	int long_only, _getopt_data* d, int posixly_correct)
{
	int iPrintError = d->opterr;

	if (argc < 1)
		return -1;

	d->optarg = NULL;

	/* initialize the option string */
	if (d->optind == 0 || ! d->__init)
		optstring = getopt_initialize(argc, argv, optstring, d, posixly_correct);
	else if (optstring[0] == '-' || optstring[0] == '+')
		++optstring;

	/* don't print errors */
	if (optstring[0] == ':')
		iPrintError = 0;

	/* test whether ARGV[optind] points to a non-option argument */
#define NONOPTION_P (argv[d->optind][0] != '-' || argv[d->optind][1] == '\0')

	if (d->__nextchar == NULL || *d->__nextchar == '\0')
	{
		/* Advance to the next element.
		   Give FIRST_NONOPT & LAST_NONOPT rational values if OPTIND
		   has been moved back by the user. */
		if (d->__last_nonopt > d->optind)
			d->__last_nonopt = d->optind;
		if (d->__first_nonopt > d->optind)
			d->__first_nonopt = d->optind;

		if (d->__ordering == PERMUTE)
		{
			/* If we've just processed some options following some non-options,
			   exchange them so that the options come first. */
			if (d->__first_nonopt != d->__last_nonopt
					&& d->__last_nonopt != d->optind)
				exchange(argv, d);
			else if (d->__last_nonopt != d->optind)
				d->__first_nonopt = d->optind;

			/* Skip any additional non-options and extend the range
			   of non-options previously skipped. */
			while (d->optind < argc && NONOPTION_P)
				d->optind++;
			d->__last_nonopt = d->optind;
		} /* end if (d->__ordering == PERMUTE) */

		/* The special ARGV-element '--' means premature end of options.
		   Skip it like a null option, then exchange with previous
		   non-options as if it were an option, then skip everything
		   else like a non-option. */
		if (d->optind != argc && ! STRCMP(argv[d->optind], "--"))
		{
			d->optind++;

			if (d->__first_nonopt != d->__last_nonopt
					&& d->__last_nonopt != d->optind)
				exchange(argv, d);
			else if (d->__first_nonopt == d->__last_nonopt)
				d->__first_nonopt = d->optind;
			d->__last_nonopt = d->optind = argc;
		} /* end if (d->optind != argc && ! STRCMP(argv[d->optind], "--")) */

		/* If we have done all the ARGV-elements, stop the scan
		   and back over any non-options that we skipped and
		   permutted, so the caller will digest them. */
		if (d->optind == argc)
		{
			/* Set the next-arg-index to point at the non-options
			   that we previously skipped, so the caller will digest them.  */
			if (d->__first_nonopt != d->__last_nonopt)
				d->optind = d->__first_nonopt;
			return -1;
		} /* end if (d->optind == argc) */

		/* If we have come to a non-option and did not permute it,
		   either stop the scan or describe it to the caller and pass it by. */
		if (NONOPTION_P)
		{
			if (d->__ordering == REQUIRE_ORDER)
				return -1;
			d->optarg = argv[d->optind++];
			return 1;
		}

		/* We've found another option. Check whether it might be
		   a long option. */
		if (longopts)
		{
			if (argv[d->optind][1] == '-')
			{
				/* "--foo" is always a long option. The special option
				   "--" was handled above. */
				d->__nextchar = argv[d->optind] + 2;
				/* TODO: add process_long_option() */
			}

			/* If long_only and the ARGV-element has the form "-f",
			   where f is a valid short option, don't consider it
			   an abbreviated form of a long option that starts with f.
			   Otherwise there would be no way to give the -f short option. */
			if (long_only && (argv[d->optind][2]
				|| ! STRRCHR(optstring, argv[d->optind][1])))
			{
				int iCode;
				d->__nextchar = argv[d->optind] + 1;
				iCode = 0; /* TODO: add process_long_option() */
				if (iCode != -1)
					return iCode;
			}
		} /* end if (longopts) */

		/* not a long option. skip the initial punctuation */
		d->__nextchar = argv[d->optind] + 1;
	} /* end if (d->__nextchar == NULL || *d->__nextchar == '\0') */

	/* look at and handle the next short option character */
	{
		char c = *d->__nextchar++;
		const char* temp = STRCHR(optstring, c);

		/* increment 'optind' when we start to process its last character */
		if (*d->__nextchar == '\0')
			++(d->optind);

		if (temp == NULL || c == ':' || c == ';')
		{
			if (iPrintError)
				eprintf("%s: invalid option -- '%c'\n", argv[0], c);
			d->optopt = c;
			return '?';
		}

		if (temp[1] == ':')
		{
			if (temp[2] == ':')
			{
				/* this is an option that accepts an argument optionally */
				if (*d->__nextchar != '\0')
				{
					d->optarg = d->__nextchar;
					d->optind++;
				}
				else
					d->optarg = NULL;
			}
			else
			{
				/* this is an option that requires an argument */
				if (*d->__nextchar != '\0')
				{
					d->optarg = d->__nextchar;
					/* Advance to the next ARGV-element, because the
					   rest of this element is the argument to this option. */
					d->optind++;
				}
				else if (d->optind == argc)
				{
					if (iPrintError)
						eprintf("%s: option requires an argument -- '%c'\n",
								argv[0], c);
					d->optopt = c;
					c = (optstring[0] == ':') ? ':' : '?';
				}
				else
					d->optarg = argv[d->optind++];
				d->__nextchar = NULL;
			}
		}
		return c;
	}

#undef NONOPTION_P

	return -1;
}

static int getopt_internal (int argc, char** argv, const char* optstring,
	const GetOpt_Option* longopts, int* longind, int long_only,
	int posixly_correct)
{
	int iResult;

	_Data.optind = optind;
	_Data.opterr = opterr;

	iResult = getopt_internal_r(argc, argv, optstring, longopts, longind,
								long_only, &_Data, posixly_correct);

	optind = _Data.optind;
	optarg = _Data.optarg;
	optopt = _Data.optopt;

	return iResult;
}

int my_getopt (int argc, char** argv, const char *optstring)
{
	return getopt_internal(argc, argv, optstring, NULL, 0, 0, 0);
}
