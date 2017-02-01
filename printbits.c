
#include <stdio.h>

/**
 * print binary representation of the n least
 * significant bits of x. 
 */
void printbits(unsigned x, int n)
{
	for (; n>0; n--)
		putchar(x & 1 << n-1 ? '1' : '0');
}

