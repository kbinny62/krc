/**
 *   This is a progressive implementation of the 'sort' program
 *   from sections 5.6 and 5.11 and subsumes solutions to the
 *   problems:
 *   	- 5.7 (p. 110), using external (git submodule) library 'argv'
 *   	- 5.14, 5.15, 5.16, and 5.17 (p. 121)
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>

#ifndef	LINE_MAX
#define	LINE_MAX	4096
#endif

#include "getline.h"
#include "argv.h"
#include "qsort.h"

void writelines(const char * const *argv) {
	while (*argv)
		fprintf(stdout, "%s\n", *argv++);
}

const cmpFun sortCallback = strcmp;

int main(int argc, char *argv[])
{
	struct argv *lines = argv_init();
	char buf[LINE_MAX];

	while (get_line(buf, LINE_MAX)) {
		argv_append(lines, buf);
	}

	quicksort(lines->argv, 0, lines->argc-1, sortCallback);

	writelines(lines->argv);

	argv_free(lines);
}
