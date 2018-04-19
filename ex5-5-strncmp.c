/**
 *  From ISO/IEC 9899:201x (N1570):
 *  The strncmp function compares not more than n characters (characters that
 *  follow a null character are not compared) from the array pointed to by s1
 *  to the array pointed to by s2.
 *
 *  The strncmp function returns an integer greater than, equal to, or less
 *  than zero, accordingly as the possibly null-terminated array pointed to
 *  by s1 is greater than, equal to, or less than the possibly null-terminated
 *  array pointed to by s2.
 */

int strncmp(const char *s1, const char *s2, unsigned long n) {
	if (n == 0)
		return 0;
	while (*s1 && *s2 && *s1 == *s2 && n)
		if (--n)
			s1++, s2++;
	return *s1 - *s2;
}

#if defined (_TEST_)
#include <stdio.h>
int main() {

	const size_t MAX_LINE = 256;
	char line1[MAX_LINE],
	     line2[MAX_LINE];
	int  n = 0;

	fputs("str1: ", stdout); fflush(stdout);
	fgets(line1, MAX_LINE, stdin);
	fputs("str2: ", stdout); fflush(stdout);
	fgets(line2, MAX_LINE, stdin);
	fputs("n: ", stdout); fflush(stdout);
	fscanf(stdin, "%u", &n);

	fprintf(stdout, "strncmp(str1, str2, %u) = %i\n", n,
			strncmp(line1, line2, n));
	
	return 0;
}
#endif	/* _TEST_ */
