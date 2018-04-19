/**
 * A small ISO C99 library to manage simple dynamic string vectors a-la argv
 * Author: <carlovalenti@ac.c4q.nyc>, public domain
 *
 * Interface:
 *   struct argv *argv_init()
 *   argv_find(struct argv*, const char *string)
 *   argv_append(struct argv*, const char *string)
 */

#ifndef _ARGV_H
#define _ARGV_H

#include <stdbool.h>
#include <unistd.h>

struct argv {
    char **argv;  // memory buffer
    size_t argc;        // currently held item count
    size_t vector_size; // actual malloced size
};


/**
 *  Initialize a brand new vector and return associated pointer handle
 */
struct argv *argv_init();

/**
 *   Return position of first occurrence of argv in vector, -1 otherwise
 */
long argv_find(struct argv *, const char *);

/**
 *   Insert a copy of string argument into argv
 */
void argv_append(struct argv *, const char *);

/**
 *  Frees up dynamically allocated resources
 */
void argv_free(struct argv *);

#endif /* _ARGV_H */

