/**
 *  From ISO/IEC 9899:201x (N1570):
 *  The strncat function appends not more than n characters (a null character and
 *  characters that follow it are not appended) from the array pointed to by s2
 *  to the end of the string pointed to by s1. The initial character of s2 overwrites
 *  the null character at the end of s1. A terminating null character is always
 *  appended to the result.
 *
 *  If copying takes place between objects that overlap, the behavior is undefined.
 *  The strncat function returns the value of s1.
 */

char *strncat(char *dest, const char *src, unsigned long n)
{
	char * const retv = dest;

	while (*dest)
		dest++;
	while (n--)
		*dest++ = *src++;
	*dest = '\0';

	return retv;
}

#if defined (_TEST_)
#include <stdio.h>
int main() {
	char s1[16];

	__builtin_strncpy(s1, "123", sizeof(s1));
	fprintf(stdout, "%s\n", strncat(s1, "456", sizeof(s1)-__builtin_strlen(s1)-1));
	
	__builtin_strncpy(s1, "", sizeof(s1));
	fprintf(stdout, "%s\n", strncat(s1, "456", sizeof(s1)-__builtin_strlen(s1)-1));

	__builtin_strncpy(s1, "123456789012345", sizeof(s1));
	fprintf(stdout, "%s\n", strncat(s1, "", sizeof(s1)-__builtin_strlen(s1)-1));

	
	return 0;
}
#endif	/* _TEST_ */
