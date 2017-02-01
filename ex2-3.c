
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int htoi(const char *s)
{
	int retv = 0, c;

	if (s == NULL)
		return 0;
	if (strlen(s) >= 2 && s[0] == '0' &&
		(s[1] == 'x' || s[1] == 'X'))
			s += 2;	/* skip 0x */
	while ((c = *s++) && isxdigit(c)) {
		c = toupper(c);
		retv <<= 4;
		retv |= isdigit(c) ? c - '0' : c-'A' + 0xA;
	}

	return retv;
}

int main(int argc, char *argv[])
{
	while (--argc) {
		fprintf(stdout, "%i\n", htoi(*++argv));
	}
	return 0;
}

