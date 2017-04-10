/**
 * ex4-1: write the function strrindex(s,t) which returns
 * the position of the rightmost occurrence of t in s, or
 * -1 if there is none.
 */

#include <stdio.h>
#include <string.h>

/* from `grep' */
int strindex(const char s[], const char t[])
{
	int i,j,k;

	for (i=0; s[i] != '\0'; i++) {
		for (j=i, k=0; t[k] != '\0' && s[j] == t[k]; j++, k++)
			;
		if (k>0 && t[k] == '\0')
			return i;
	}
	return -1;
}

int strrindex(const char *s, const char *t)
{
	const char *p = s;

	while (*p)
		p++;
	if (p == s)
		/* empty string */
		return -1;
	else if (s == t)
		/* string matches itself */
		return 0;
	else while (p >= s) {
		int i = strindex(p, t);
		if (i != -1)
			return p-s;
		else
			p--;
	}
	return -1;

}

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s HAYSTACK NEEDLE\n", argv[0]);
		return -1;
	} else {
		int i = strrindex(argv[1], argv[2]);
		fprintf(stdout, "%d\n", i);
	}
}

