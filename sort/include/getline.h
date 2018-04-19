#ifndef	_GETLINE_H
#define _GETLINE_H

#include <stdio.h>

static int getline(char s[], int maxlen)
{
	int c, i;

	i = 0;
	while ((c = getchar()) != EOF) {
		if (c == '\n' || c == EOF)
			break;
		if (i >= maxlen-1)
			break;
		s[i++] = c;
	}

	s[i] = '\0';
	return i;
}

#endif	/* _GETLINE_H */
