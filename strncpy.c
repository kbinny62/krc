/**
 *  Quoting standard ISO/IEC 9899:201x Committee Draft — April 12, 2011 N15:
 *  The strncpy function copies not more than n characters (characters that
 *  follow a null character are not copied) from the array pointed to by s2 to
 *  the array pointed to by s1. If copying takes place between objects that
 *  overlap, the behavior is undefined.
 *  If the array pointed to by s2 is a string that is shorter than n characters,
 *  null characters are appended to the copy in the array pointed to by s1,
 *  until n characters in all have been written.
 *
 *  The strncpy function returns the value of s1.
 */

#include <stdio.h>

char *strncpy(char *dest, const char *src, size_t n) {
	char *const retv = dest;
	while (n--) {
		*(dest++) = *src == '\0' ? '\0' : *(src++);
	}

	return retv;
}

/* Compile with -D_TEST_ -fno-builtin */
#if defined(_TEST_)
int main() {
	char d[3], s[3] = "12";
	fprintf(stdout, "%s\n", strncpy(d, "12", sizeof(d)));
	fprintf(stdout, "%s\n", strncpy(d, "1", sizeof(d)));
	fprintf(stdout, "%s\n", strncpy(d, "1234", sizeof(d)));
	fprintf(stdout, "%s\n", strncpy(d, "", sizeof(d)));
	return 0;
}
#endif /* _TEST */
