
/**
 *  NOTE: This program produces GraphViz output by default. A sample SVG rendering
 *        of such output is attached to the repository along with the data used to
 *        generate it (main text of The U.S Declaration of Independence).
 *        Plain ASCII output can be forced with the '-a' command line option.
 *
 */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "external/argv/argv.c"

/**
 * @const default prefix length, runtime value alterable through -n command line option
 */
static struct argv *noisevec = NULL;

const char NOISEFILE[] = "DATASETS/ex6-3-noisewords.txt"; /* relative to curdir */

struct tnode {
	char *word;
	struct argv *linerefs; /* lines number xrefs as string argv[], not ideal from a storage
				  cost perspective, but at least extensible if needed. */
	struct tnode *left;
	struct tnode *right;
};

/**
 *  This is an adaptation of @function binsearch() from ex6-1
 *  Used for filtering out 'noise' words (the, and, ...)
 *  Assumes the invariant of argv being presorted
 *  @returns -1 for Not Found, or index in tab otherwise
 */
int binsearch(char *word, char **tab, int n, int(*fncmp)(const char*, const char *)) {
	const int mid = n / 2;
	int cmp;
	if (n <= 0)
		return -1;
	if ((cmp = fncmp(tab[mid], word)) == 0)
		return mid;
	else if (n == 1)
		return -1;
	else if (cmp > 0)
		return binsearch(word, tab, mid, fncmp);
	else {
		int bs = binsearch(word, tab+mid, n-mid, fncmp);
		return bs != -1 ? mid+bs : bs;
	}
}
/**
 * Only returns words and newlines, punctuation etc...
 * @returns 1 for success, 0 for failure
 * @param szlim - size of buffer in bytes, including NUL terminator
 */
int getword(FILE *fp, char *word, unsigned szlim)
{
	char *w = word;
	int c;

	assert(szlim > 1);
	while ((c = fgetc(fp)) != EOF) {
		if (isalpha(c) || c == '\n')
			break;
	}

	if (c == EOF)
		return 0;
	else if (c == '\n')
		return *w++ = '\n', *w = '\0', 1;

	*w++ = c;
	for ( ; szlim-- > 1; w++) {
		if (!isalpha(*w = fgetc(fp))) {
			ungetc(*w, fp);
			break;
		}
	}

	*w = '\0';
	return 1;
}

struct tnode *talloc() {
	struct tnode *retv = malloc(sizeof(struct tnode));
	assert(retv != NULL);
	return retv;
}

struct tnode *addtree(struct tnode *n, const char *word, unsigned cur_line) {
	char buf_linenum[32];
	int cmp;

	snprintf(buf_linenum, sizeof buf_linenum, "%u", cur_line);

	if (n == NULL) {
		n = talloc();
		n->word = malloc(sizeof(char) * BUFSIZ);
		n->linerefs = argv_init();
		assert(n != NULL && n->word != NULL && n->linerefs != NULL);
		strncpy(n->word, word, BUFSIZ-1);
		n->word[BUFSIZ-1] = '\0';
		argv_append(n->linerefs, buf_linenum);
		n->left = n->right = NULL;
	} else if ((cmp = strcasecmp(word, n->word)) == 0) {
		if (argv_find(n->linerefs, buf_linenum) == -1)
			/* multiple occurrence on the same line count once */
			argv_append(n->linerefs, buf_linenum);
	} else if (cmp > 0) {
		n->right = addtree(n->right, word, cur_line);
	} else {
		n->left = addtree(n->left, word, cur_line);
	}

	return n;
}

static inline void INDENT(FILE *fp, unsigned level) {
	while (level--) fputc(' ', fp);
}

/**
 *  @param{bool} is_graphviz - controls whether GraphViz or plain ASCII gets generated
 */
void treeprint(FILE *fp, struct tnode const *root, unsigned level, int is_graphviz) {
	if (!root)
		return;

	if (is_graphviz) {
		char **argv = root->linerefs->argv;
		time_t t = time(NULL);
		
		if (level == 0) /* file header */
			fprintf(fp, "/* Machine generated GraphViz input\n"
				    " * %s" /* \n supplied by ctime() */
				    " */\n"
				    "graph {\nranksep=5;\n", ctime(&t));

		INDENT(fp, level+1);
		fprintf(fp, "\"%s\"\n", root->word);

		if (*argv != NULL) { /* cross-ref line numbers */
			INDENT(fp, level+1);
			fprintf(fp, "\"%s\" -- { node [shape=box]; ", root->word);
			while (*argv)
				fprintf(fp, "\"%s\"; ", *argv++);
			fprintf(fp, " }\n");
		}
	} else { /* plain ASCII output */
		fprintf(fp, "<GROUP '%s'>: [", root->word);
		for (char **argv = root->linerefs->argv; *argv != NULL; argv++)
			fprintf(fp, "'%s'%s", *argv, *(argv+1) ? ", " : "");
		fprintf(fp, "]\n");
	}

	// Recurse into lower levels, all output at the same level == flat-hierarchy graph
	treeprint(fp, root->left,  /* level + */ 1, is_graphviz);
	treeprint(fp, root->right, /* level + */ 1, is_graphviz);
	
	if (is_graphviz && level == 0) {
		fprintf(fp, "} /* graph */\n");
	}
}

/**
 *  Cleanup of tree and associated data structures
 */
void treefree(struct tnode **root) {
	if (!(root && *root))
		return;
	treefree(&(*root)->left);
	treefree(&(*root)->right);
	argv_free((*root)->linerefs);
	free((*root)->word);
	free(*root);
	*root = NULL;
}

/**
 * helper @function for standard qsort */
static int sort_pred(const char **s1, const char **s2) {
	return strcasecmp(*s1, *s2);
}

int main(int argc, char *argv[]) {
	struct tnode *root = NULL;
	char word[BUFSIZ];
	unsigned doGraphviz = 1;
	unsigned line_num = 1;
	int opt;

	while ((opt = getopt(argc, argv, "a")) != -1) {
		switch (opt) {
			case 'a':
				doGraphviz = 0;
				break;
			default:
				fprintf(stderr,
						"Usage: %s [-a]\n"
						"          -a   plain ASCII output\n",
						argv[0]);
				break;
		}
	}

	if (noisevec == NULL) { /* initialize once on startup */
		FILE *noisefile = fopen(NOISEFILE, "r");
		noisevec = argv_init();

		if (noisefile == NULL) {
			fprintf(stderr, "WARNING: Unable to noise words list file '%s'.\n", NOISEFILE);
			argv_append(noisevec, "and"); argv_append(noisevec, "the");
			argv_append(noisevec, "but"); argv_append(noisevec, "not");
		} else {
			while (getword(noisefile, word, BUFSIZ))
				if (*word != '\n' && argv_find(noisevec, word) == -1)
					argv_append(noisevec, word);
			fclose(noisefile);
		}

		qsort(noisevec->argv, noisevec->argc, sizeof *noisevec->argv, sort_pred);
	}

	while (getword(stdin, word, BUFSIZ)) {
		if (*word == '\n')
			line_num++;
		else if (binsearch(word, noisevec->argv, noisevec->argc, strcasecmp) == -1)
			root = addtree(root, word, line_num);
	}

	treeprint(stdout, root, 0, doGraphviz);
	argv_free(noisevec);
	treefree(&root);
	return 0;
}
