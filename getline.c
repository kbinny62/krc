#include <stdio.h>

int getline(char s[], int maxlen)
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

#define	MAXLINE	1000

void copy(char dest[], char src[])
{
	register int i = 0;
	while ((dest[i] = src[i]) != '\0')
		i++;
}

int main()
{
	int len, max;
	char line[MAXLINE];
	char longest[MAXLINE];

	max = 0;
	while (len = getline(line, MAXLINE))
		if (len > max) {
			copy(longest, line);
			max = len;
		}

	if (max > 0)
		printf("%s", longest);

	return 0;
}

