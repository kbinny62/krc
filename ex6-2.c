
/**
 *  NOTE: instead of displaying alphanumerically-sorted word groups with common prefixes
 *        as requested in the problem statement, this program produces GraphViz output by
 *        default. A sample SVG rendering of such output is attached to the repository
 *        along with the data used to generate it (50 first words from Webster's dict).
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
const unsigned DEFAULT_PFXLEN = 6;

struct tnode {
	char *prefix;
	struct argv *wordgroup;
	/* unsigned count; */
	struct tnode *left;
	struct tnode *right;
};

/**
 * This is ex6.1's @function getword(), skips comments and string literals
 * Newline (\n) is explicity handled as its own token
 * @returns 1 for success, 0 for failure
 */
int getword(char *word, int lim)
{
	int c, inside_comment = 0, inside_strlit = 0;
	char *w = word;
	while ((c = getchar()) != EOF) {
		int peek;
		if (c == '/' && !inside_strlit) {
			if ((peek = getchar()) == '*')
				inside_comment = 1;
			else
				ungetc(peek, stdin);
		} else if (c == '*' && inside_comment) {
			if ((peek = getchar()) == '/')
				inside_comment = 0;
			else
				ungetc(peek, stdin);
		} else if (c == '"' && !inside_comment) {
			inside_strlit ^= 1; /* bitwise invert */
		} else if (inside_comment || inside_strlit) {
			continue;
		} else if (!isspace(c) || c == '\n')
			break;
	}
	if (c != EOF)
		*w++ = c;
	else
		return 0;
	if (!(isalpha(c) || c == '_')) {
		*w = '\0';
		return 1;
	}
	for ( ; --lim > 0; w++) {
		if (!isalnum(*w = getchar()) && *w != '_') {
			ungetc(*w, stdin);
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

struct tnode *addtree(struct tnode *n, const char *word, unsigned prefix_len) {
	int cmp;
	if (n == NULL) {
		n = talloc();
		n->prefix = malloc(sizeof(char) * prefix_len + 1);
		n->wordgroup = argv_init();
		assert(n != NULL && n->prefix != NULL && n->wordgroup != NULL);
		strncpy(n->prefix, word, prefix_len);
		n->prefix[prefix_len] = '\0';
		n->left = n->right = NULL;
	} else if ((cmp = strncmp(word, n->prefix, prefix_len)) == 0) {
		if (argv_find(n->wordgroup, word) == -1)
			argv_append(n->wordgroup, word);
	} else if (cmp > 0) {
		n->right = addtree(n->right, word, prefix_len);
	} else {
		n->left = addtree(n->left, word, prefix_len);
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
		char **argv = root->wordgroup->argv;
		time_t t = time(NULL);
		
		if (level == 0) /* file header */
			fprintf(fp, "/* Machine generated GraphViz input\n"
				    " * %s" /* \n supplied by ctime() */
				    " */\n"
				    "graph {\n", ctime(&t));

		INDENT(fp, level+1);
		fprintf(fp, "\"%s\"\n", root->prefix);

		if (*argv != NULL) { /* wordgroup */
			INDENT(fp, level+1);
			fprintf(fp, "\"%s\" -- { node [shape=box]; rank=same;", root->prefix);
			while (*argv)
				fprintf(fp, "\"%s\"; ", *argv++);
			fprintf(fp, " }\n");
		}

		if (root->left || root->right) { /* descendants? give their own subgraph */
			INDENT(fp, level+1);
			fprintf(fp, "subgraph { /* level=%u */\n", level);
		}
	} else { /* plain ASCII output */
		fprintf(fp, "<GROUP '%s'>: [", root->prefix);
		for (char **argv = root->wordgroup->argv; *argv != NULL; argv++)
			fprintf(fp, "'%s'%s", *argv, *(argv+1) ? ", " : "");
		fprintf(fp, "]\n");
	}

	// Recurse into lower levels
	treeprint(fp, root->left, level+1, is_graphviz);
	treeprint(fp, root->right, level+1, is_graphviz);
	
	if (is_graphviz) {
		if (root->left || root->right) {
			// close nested subgraph
			INDENT(fp, level+1);
			fprintf(fp, "} /* subgraph level=%u, pfx='%s' */\n", level, root->prefix);

			if (root->left) {
				INDENT(fp, level+1);
				fprintf(fp, "\"%s\" -- \"%s\"\n", root->prefix, root->left->prefix);
			}

			if (root->right) {
				INDENT(fp, level+1);
				fprintf(fp, "\"%s\" -- \"%s\"\n", root->prefix, root->right->prefix);
			}
		}
		// close graph
	       	if (level == 0)
			fprintf(fp, "} /* digraph */\n");
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
	argv_free((*root)->wordgroup);
	free((*root)->prefix);
	free(*root);
	*root = NULL;
}


int main(int argc, char *argv[]) {
	struct tnode *root = NULL;
	char word[BUFSIZ], *eptr;
	unsigned prefix_len = DEFAULT_PFXLEN;
	unsigned doGraphviz = 1;
	int opt;

	while ((opt = getopt(argc, argv, "n:a")) != -1) {
		switch (opt) {
			case 'a':
				doGraphviz = 0;
				break;
			case 'n':
				prefix_len = strtoul(optarg, &eptr, 0);
				if (eptr == optarg || prefix_len < 1) {
					fprintf(stderr, "%s: unable to parse a valid prefix length (n >= 1)", argv[0]);
					prefix_len = DEFAULT_PFXLEN;
				}
				break;
			default:
				fprintf(stderr,
						"Usage: %s [-a] [-n PREFIX_LEN]\n"
						"          -a   plain ASCII output\n"
						"          -n   defaults to %u\n",
						argv[0], DEFAULT_PFXLEN);
				break;
		}
	}

	while (getword(word, BUFSIZ)) {
		if (*word != '\n')
			root = addtree(root, word, prefix_len);
	}

	treeprint(stdout, root, 0, doGraphviz);
	treefree(&root);
	return 0;
}
