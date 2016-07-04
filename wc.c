/**
 *  K&R 2: wc (word count) program
 */
#include <stdio.h>

int main()
{
	int c, nl, nw, nc;
	int in_word;

	nl = nw = nc = 0;
	in_word = 0;
	while ((c = getchar()) != EOF) {
		nc++;
		nl += c == '\n' ? 1 : 0;
		if (c == ' ' || c == '\t' || c == '\n')
			in_word = 0;
		else if (!in_word) {
			nw++;
			in_word = 1;
		}
	}

	fprintf(stdout, "%d %d %d\n", nl, nw, nc);

	return 0;
}

