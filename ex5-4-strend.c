/**
 *   strend(s, t) returns 1 if the string t occurs
 *   at the end of the string s, and zero otherwise
 */
int strend(const char * const s, const char * const t) {
	const char *m = s, *n = t;
	while (*m) /* m = s.end() */
		m++;
	while (*n) /* n = t.end() */
		n++;

	/* Maintain invariant as long as chars of s and t at indices i are equal,
	 * of backwards loop and both strings have characters left */
	while (m >= s && n >= t && *m == *n) {
		m--, n--;
	}

	/* t is suffix of s iff loop terminated with no chars of t left to compare.
	 * Return 0 for any other cases. */
	return n == t - 1 ? 1 : 0;
}

#include <stdio.h>
int main()
{
	const size_t MAX_LINE = 256;
	char line1[MAX_LINE],
	     line2[MAX_LINE] ;

	fputs("str1: ", stdout); fflush(stdout);
	fgets(line1, MAX_LINE, stdin);
	fputs("str2: ", stdout); fflush(stdout);
	fgets(line2, MAX_LINE, stdin);

	fprintf(stdout, "strend(str1,str2) = %u\n", strend(line1, line2));
	return 0;
}
