
#include <stdio.h>

/**
 * print binary representation of the n least
 * significant bits of x. 
 */
void printbits(long x, unsigned n)
{
	for (; n>0; n--)
		putchar(x & 1 << n-1 ? '1' : '0');
}

