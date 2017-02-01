
#include <stdio.h>

void squeeze(char *s1, const char *s2)
{
	char *str;
	const char *p;

	for (str = s1; *s1; s1++) {
		for (p = s2; *p; p++)
			if (*s1 == *p)
				break;
		if (*p == '\0')
			*str++ = *s1;
	}
	*str = '\0';
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s STR1 STR2\n", argv[0]);
		return -1;
	}

	squeeze(argv[1], argv[2]);
	printf("%s\n", argv[1]);

	return 0;
}
