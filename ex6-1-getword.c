#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct key {
	const char *word;
	unsigned count;
} keytab[] = { /* language keywords */
	{ "auto", 0, },
	{ "break", 0, },
	{ "case", 0, },
	{ "char", 0, },
	{ "const", 0, },
	{ "continue", 0, },
	{ "default", 0, },
	{ "do", 0, },
	{ "double", 0, },
	{ "else", 0, },
	{ "enum", 0, },
	{ "extern", 0, },
	{ "float", 0, },
	{ "for", 0, },
	{ "goto", 0, },
	{ "if", 0, },
	{ "inline", 0, },
	{ "int", 0, },
	{ "long", 0, },
	{ "register", 0, },
	{ "return", 0, },
	{ "short", 0, },
	{ "signed", 0, },
	{ "sizeof", 0, },
	{ "static", 0, },
	{ "struct", 0, },
	{ "switch", 0, },
	{ "then", 0, },
	{ "typedef", 0, },
	{ "unsigned", 0, },
	{ "void", 0, },
	{ "volatile", 0, },
	{ "while", 0, },
}, ppdirtab[] = { /* preprocessor directives */
	{ "define", 0, },
	{ "elif", 0, },
	{ "else", 0, },
	{ "if", 0, },
	{ "ifdef", 0, },
	{ "include", 0, },
	{ "undef", 0, },
};

const size_t NKEYS = sizeof(keytab)/sizeof(struct key),
             NPPDIRS = sizeof(ppdirtab)/sizeof(struct key);

int getword(char *, int);
int binsearch(char *, struct key *, int);

int binsearch(char *word, struct key tab[], int n) {
	const int mid = n / 2;
	int cmp;
	if (n <= 0)
		return -1;
	if ((cmp = strcmp(tab[mid].word, word)) == 0)
		return mid;
	else if (n == 1)
		return -1;
	else if (cmp > 0)
		return binsearch(word, tab, mid);
	else {
		int bs = binsearch(word, tab+mid, n-mid);
		return bs != -1 ? mid+bs : bs;
	}
}

/**
 * Newline (\n) is returned as its own token
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

int main() {
	char word[BUFSIZ], prev_is_hash;
	size_t i;

	*word = prev_is_hash = '\0';
	while (getword(word, sizeof word)) {
		int bsearch = binsearch(word, keytab, NKEYS);
		if (prev_is_hash) {
			if ((bsearch = binsearch(word, ppdirtab, NPPDIRS)) == -1)
				fprintf(stderr, "Warning: unrecongnized pre-processor "
					" directive '%s', at file pos %zd\n", word, ftell(stdin));
			while (getword(word, sizeof word) && *word != '\n')
				/* skip rest of line */;
			prev_is_hash = 0;
		} else if (bsearch != -1)
			keytab[bsearch].count++;
		else if (*word == '#')
			prev_is_hash = 1;
	}

	/* segregate output based on content so that raw data
	 * ends up in stdout and pretty printing in stderr, in
	 * order to facilitate filtering/pipleline processing.
	 */
	for (i = 0; i < NKEYS; i++) {
		fprintf(stderr, "%-10s:", keytab[i].word); fflush(stderr);
		fprintf(stdout, " %-4d", keytab[i].count); fflush(stdout);
		fputs((i+1) % 2 == 0 || i == NKEYS-1
				? "\n"
				: "         ", stderr);
	}
	return 0;
}
