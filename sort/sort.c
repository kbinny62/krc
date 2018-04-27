/**
 *   This is a progressive implementation of the 'sort' program
 *   from sections 5.6 and 5.11 and subsumes solutions to the
 *   problems:
 *   	- 5.7 (p. 110), using external (git submodule) library 'argv'
 *   	- 5.14, 5.15, 5.16, and 5.17 (p. 121)
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#ifndef	LINE_MAX
#define	LINE_MAX	4096
#endif

#include "getline.h"
#include "argv.h"
#include "qsort.h"

struct {
	int opt_numeric;
	int opt_reverse;
} _g = {
	0,
	0,
};

static void writelines(char **argv) {
	while (*argv)
		fprintf(stdout, "%s\n", *argv++);
}

static int sortCallback(const char *s1, const char *s2) {
	int retv;

	if (_g.opt_numeric) {
		/* From IEEE Std 1003.1-2017:
		 * Restrict the sort key to an initial numeric string, consisting of optional <blank> characters,
		 * optional <hyphen-minus> character, and zero or more digits with an optional radix character and
		 * thousands separators (as defined in the current locale), which shall be sorted by arithmetic value.
		 * An empty digit string shall be treated as zero. Leading zeros and signs on zeros shall not affect
		 * ordering.
		 */
		if (strcmp(s1, s2) != 0) {
			long n1, n2;
			char *eptr;

			while (isspace(*s1))
				s1++;
			while (isspace(*s2))
				s2++;

			n1 = strtol(s1, &eptr, 0);
			if (eptr == s1)
				n1 = 0;

			n2 = strtol(s2, &eptr, 0);
			if (eptr == s2)
				n2 = 0;

			retv = n1 - n2;
		} else
			retv = 0;
	} else {
		retv = strcmp(s1, s2);
	}

	return _g.opt_reverse ? -retv : retv;
}

int main(int argc, char *argv[])
{
	struct argv *lines = argv_init();
	char buf[LINE_MAX];
	int i;

	for (i=1; i < argc; i++) {
		if (argv[i][0] == '-' && argv[i][1] != '\0') {
			int j = 1;
			while (argv[i][j] != '\0') {
				switch (argv[i][j]) {
					case 'n':
						_g.opt_numeric = 1;
						break;
					case 'r':
						_g.opt_reverse = 1;
						break;
					default:
						fprintf(stderr, "%s: unrecognized option '%c' in command line argument '%s'\n",
								*argv, argv[i][j], argv[i]);
						return -1;
				}
				j++;
			}
		} else {
			/* non-option arg */
		}
	}

	while (get_line(buf, LINE_MAX)) {
		argv_append(lines, buf);
	}

	quicksort(lines->argv, 0, lines->argc-1, sortCallback);

	writelines(lines->argv);

	argv_free(lines);
}

