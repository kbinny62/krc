/**
 *  ex5-13: tail, default n=10 with optional -n argument
 *  Also accepts file path names on the command line
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* as in 5.6, with the exception of lineptr being dyanmically allocated */
char **lineptr;
unsigned arg_n = 10, buf_index = 0;

void usage(const char *exename, const char *msg)
{
	if (msg)
		fprintf(stderr, "%s: %s\n", exename, msg);
	fprintf(stderr, "Usage: %s [-N] [-|FILE...]\n"
			"       If no FILE(s) supplied, input is stdin\n", exename);
	exit(EXIT_FAILURE);
}

inline char *strdup(const char *src) {
	return strcpy(malloc(strlen(src)+1), src);
}

inline void append_line(const char *line) {
	if (buf_index < arg_n) {
		lineptr[buf_index++] = strdup(line);
	} else {
		unsigned i;
		for (i=0; i<arg_n-1; i++) {
			if (i == 0)
				free(lineptr[0]);
			lineptr[i] = lineptr[i+1];
		}
		lineptr[arg_n-1] = strdup(line);
	}
}

int tail_file(const char *pathname)
{
	char line[BUFSIZ];
	FILE *fp;
	unsigned i;

	if ((fp = strcmp(pathname, "-") ? fopen(pathname, "r") : stdin) == NULL) {
		perror(pathname);
		return 1;
	}

	buf_index = 0;
	while (fgets(line, BUFSIZ, fp))
		append_line(line);

	for (i=0; i<buf_index; i++) {
		printf("%s", lineptr[i]);
		free(lineptr[i]);
		lineptr[i] = NULL;
	}
	fclose(fp);
	return 0;
}

int main(int argc, char *argv[])
{
	unsigned firstarg = 1;
	int retv = EXIT_SUCCESS;

	if (argc > 1) {
		if (*argv[1] == '-' && isdigit(argv[1][1])) {
			arg_n = (unsigned)strtoul(argv[1]+1, (char**)NULL, 10);
			if (arg_n == 0)
				usage(argv[0], "-N must be non-zero");
			firstarg++;
		} else if (*argv[1] == '-' && argv[1][1] != '\0')
			usage(argv[0], "-N requires a numeric argument");
		else
			/* treat '-' as a filename (stdin) */;
	}

	lineptr = malloc(sizeof(char*)*arg_n);
	memset(lineptr, (int)NULL, sizeof(char*)*arg_n);
	if (firstarg == argc)
		retv |= tail_file("-");
	else while (firstarg < argc)
		retv |= tail_file(argv[firstarg++]);
	return retv;
}

