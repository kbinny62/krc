
#include <stdio.h>

/**
 * these functions are defined each in their own separate files
 * SRCS+=printbits.c getbits.c setbits.c
 */
extern void printbits(unsigned x, unsigned n);
extern unsigned getbits(unsigned x, int p, int n);
extern unsigned setbits(unsigned x, int p, int n, unsigned y);

int main(int argc, char *argv[])
{
	unsigned x, y;
	int p, n;
	printf("Enter space-delimited values for 'x, p, n, y': ");
	scanf("%i %i %i %i", &x, &p, &n, &y);
	printbits(setbits(x, p, n, y), 16); putchar('\n');
}

