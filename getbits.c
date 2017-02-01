
#include <assert.h>

/**
 * n bit fields of x, starting at position p
 * right-shifted */
unsigned getbits(unsigned x, int p, int n)
{
	assert( p >= 0 && n > 0 && p+1-n >= 0 );
	return ( x >> (p+1-n) & ~(~0 << n) );
}

