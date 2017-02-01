
#include <assert.h>

/**
 * copy n LSBs from y unto x at position p */
unsigned setbits(unsigned x, int p, int n, unsigned y)
{
	assert( n <= p+1 );
	long bmask = ~(~0 << n) << p+1-n;
	return x & ~bmask |
		bmask & (getbits(y, n-1, n) << p+1-n);
}

