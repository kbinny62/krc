/**
 *  K&R 2: Exercise 1-9
 *  Write a program to copy its input to its output, replacing each
 *  string of one or more blanks by a single blank.
 */

#include <stdio.h>

int main()
{
	int c;

	while ((c = getchar()) != EOF) {
		if (c == ' ' || c == '\t')
			while (c = getchar())
				if (c != ' ' && c != '\t') {
					putchar(' ');
					break;
				}

		if (c == EOF) /* EOF from inner loop */
			break;
		else
			putchar(c);
	}

	return 0;
}

