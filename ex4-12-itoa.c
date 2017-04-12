/**
 * ex4-12: recursive itoa
 */

#include <stdio.h>

char *itoa_iter(int, char *);
char *itoa(int n, char *s)
{
	itoa_iter(n, s);
	return s;
}

char *itoa_iter(int n, char *s)
{
	if (n < 0) {
		*s++ = '-';
		n = -n;
	}

	if (n / 10)
		s = itoa_iter(n / 10, s);
	*s++ = n % 10 + '0';
	*s = '\0';
	return s;
}

int main()
{
	char buf[0x1000];

	while (1) {
		int n;
		scanf("%d", &n);
		itoa(n, buf);
		puts(buf);
	}
}

