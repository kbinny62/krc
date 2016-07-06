/**
 *  K&R 2: 1.20: detab
 *  Replace tabs with equivalent spaces.
 */
#include <stdio.h>

#define	TABSTOP	8

int main()
{
	int cur_col = 1;
	int c, x;

	while ((c = getchar()) != EOF) {
		switch (c) {
			case '\n':
				putchar(c);
				cur_col = 1;
				break;
			case '\t':
				for (x = 1+TABSTOP-cur_col%TABSTOP; x > 0; x--) {
					putchar(' ');
					cur_col++;
				}
				break;
			default:
				putchar(c);
				cur_col++;
		}
	}

	return 0;
}

