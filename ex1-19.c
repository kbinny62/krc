#include <stdio.h>

unsigned int _getline(char s[], unsigned int maxlen)
{
	unsigned int i;
	int c;

	i = 0;
	while ((c = getchar()) != EOF && --maxlen) {
		s[i++] = c;
		if (c == '\n')
			break;
	}

	s[i] = '\0';
	return i;
}

void reverse(char s[])
{
	char *e;
	for (e = s; *e != '\0'; e++);
	if (*(e-1) == '\n')
		e--;
	while (s < --e) { /* swap */
		*e = *e ^ *s;
		*s = *e ^ *s;
		*e = *e ^ *s;
		s++;
	}
}

int main()
{
	char line[100];
	while (_getline(line, sizeof(line)) != 0) {
		reverse(line);
		fputs(line, stdout);
	}
	return 0;
}

