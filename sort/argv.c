/**
 * A small ISO C99 library to manage simple dynamic string vectors a-la argv
 * Author: <carlovalenti@ac.c4q.nyc>, public domain
 *
 * Interface:
 *   struct argv *argv_init()
 *   argv_find(struct argv*, const char *string)
 *   argv_append(struct argv*, const char *string)
 */

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "argv.h"

#define    ARGV_ALLOC_GRAIN    0x0F  // grow by multiples of this many entries

struct argv *argv_init()
{
	struct argv *newargv = (struct argv*)malloc(sizeof(struct argv));
	newargv->vector_size = 1;
	newargv->argc = 0;
	newargv->argv = (char**)malloc(sizeof(char*) * newargv->vector_size);
	newargv->argv[0] = NULL; // end of vector sentinel
	return newargv;
}

long argv_find(struct argv *argv, const char *string)
{
	assert(argv != NULL && argv->argv != NULL);
	char **p = argv->argv;
	if (string == NULL)
		return -1;
	while (*p != NULL) {
		if (strcmp(*p, string) == 0)
			return p - argv->argv;
		p++;
	}
	return -1;
}

/**
 * internal, manages dynamic memory growth for storage
 */
static void argv_grow(struct argv *argv, size_t elements)
{
	argv->vector_size = (argv->vector_size + elements + ARGV_ALLOC_GRAIN) & ~ ARGV_ALLOC_GRAIN;
	argv->argv = (char**)realloc(argv->argv, sizeof(char*) * argv->vector_size);
}

void argv_append(struct argv *argv, const char *string)
{
	assert(argv != NULL && argv->argv != NULL);
	if (argv->argc >= argv->vector_size-1)
		argv_grow(argv, 1);
	argv->argv[argv->argc++] = strdup(string);
	argv->argv[argv->argc] = NULL;
}

void argv_free(struct argv *argv)
{
	if (!argv)
		return;
	if (argv->argv) {
		char ** const tmp = argv->argv;
		while (argv->argc--)
			free(*(argv->argv++));
		free(tmp);
	}
	free(argv);
}

