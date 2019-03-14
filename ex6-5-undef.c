/**
 *  Besides a different hash function, uses some C99+ syntax
 *  Also checked against memory leaks
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASHTAB_SIZE	(1<<7)	/* 128 entries */

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

static struct nlist *hashtab[HASHTAB_SIZE];

static unsigned hash(const char *s) {
	unsigned retv = 0;
	while (*s)
		retv ^= *s++;
	return retv & (HASHTAB_SIZE-1);
}

static struct nlist *lookup(const char *name) {
	for (struct nlist *p = hashtab[hash(name)] ; p != NULL; p = p->next)
		if (strcmp(name, p->name) == 0)
			return p;
	return NULL;
}

struct nlist *install(const char *name, const char *defn) {
	struct nlist *np = lookup(name);
	if (np != NULL) {
		free(np->defn);
		np->defn = strdup(defn);
	} else {
		np = malloc(sizeof(struct nlist));
		np->next = hashtab[hash(name)];
		hashtab[hash(name)] = np;
		np->name = strdup(name);
		np->defn = strdup(defn);
	}
	return np;
}

/**
 *  Debugging routine
 */
static void dump(struct nlist *tab[], size_t sz) {
	if (sz == 0) {
		struct nlist *plist = *tab;
		while (plist != NULL) {
			fprintf(stderr, "<%s, %s> ", plist->name, plist->defn);
			plist = plist->next;
		}
	} else for (size_t i = 0; i < sz; i++) {
		if (tab[i] != NULL) {
			fprintf(stderr, "hash: %#x, list: ", i);
			dump(tab+i, 0);
			fputc('\n', stderr);
		}
	}
}

void cleanup() {
	fprintf(stderr, "%s:%s+%u ...\n", __func__, __FILE__, __LINE__);
	for (size_t i = 0; i < HASHTAB_SIZE; i++) {
		struct nlist *np = hashtab[i];
		while (np) {
			struct nlist * const nnextp = np->next;
			free(np->name);
			free(np->defn);
			free(np);
			np = nnextp;
		}
	}
}

int undef(const char *name) {
	struct nlist *np = hashtab[hash(name)];
	while (np) {
		if (strcmp(np->name, name) == 0) { /* head of list */
			hashtab[hash(name)] = np->next;
			free(np->name);
			free(np->defn);
			free(np);
			return 1;
		} else if (np->next && strcmp(np->next->name, name) == 0) {
			struct nlist * const saved_nextp = np->next->next;
			free(np->next->name);
			free(np->next->defn);
			free(np->next);
			np->next = saved_nextp;
			return 1;
		} else
			np = np->next;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	atexit(cleanup);

	while (argc > 2) {
		install(argv[1], argv[2]);
		argv += 2;
		argc -= 2;
	}

	{
		char *line = NULL, *tok = NULL;
		size_t len = 0;
		ssize_t nread;

		fprintf(stderr, "Enter undefines, separated by blanks/newlines...\n");

		while ((nread = getline(&line, &len, stdin)) != -1) {
			while ((tok = strtok(tok ? NULL : line, " \t\n")) != NULL) {
				fprintf(stderr, "Undefining '%s'\n", tok);
				undef(tok);
			}
		}

		if (line)
			free(line);
	}

	dump(hashtab, HASHTAB_SIZE);
}

