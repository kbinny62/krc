/**
 *  ex3-3: expand character ranges regex-style
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void expand(const char *s1, char *s2)
{
	char c, prev_char = 0;
	int prev_isendpoint = 0;

	while (c = *s1++) {
		if (c == '-') {
			if (prev_char == 0 || *s1 == '\0')
				*s2++ = '-';
			else if (prev_isendpoint) {
				fprintf(stderr, "error: illegal shared endpoint\n");
				break;;
			} else {
				register char x;
				for (x=prev_char; x<=*s1; x++)
					*s2++ = x;
				prev_isendpoint = 1;
				s1++;
			}
		} else {
			if (prev_char != 0 && !prev_isendpoint)
				*s2++ = prev_char;
			prev_char = c;
			prev_isendpoint = 0;
		}
	}

	if (prev_char && !prev_isendpoint)
		*s2++ = prev_char;
	*s2 = '\0';
}


int main(int argc, char *argv[])
{
	argv++;
	while (--argc) {
		char *buf = malloc(4096); /* XXX */
		expand(*argv++, buf);
		fprintf(stdout, "%s\n", buf);
		free(buf);
	}

	return 0;
}

