#include <stdio.h>

int any(const char *s1, const char *s2)
{
	int idx;
	for (idx = 0; s1[idx] != '\0'; idx++) {
		const char *p;
		for (p = s2; *p; p++)
			if (*p == s1[idx])
				break;
		if (*p != '\0')
			return idx;
	}

	return -1;
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s STR1 STR2\n", argv[0]);
		return -1;
	}
	fprintf(stdout, "%i\n", any(argv[1], argv[2]));
	return 0;
}

