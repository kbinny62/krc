
#include <stdio.h>
#include <stdint.h>	/* C99: size_t */
#include <stdlib.h>	/* malloc */
#include <string.h>
#include <assert.h>

#define	BUF_SIZE_GRAIN	32

/**
 *  Input a word up to wd_size chars, including the terminating NUL
 *  If wd_size is 0, storage is allocated for the word as needed.
 *  The caller should handle cleanup
 */
char* getword(FILE *f, size_t wd_size)
{
	size_t len = 0, buf_len;
	char *buf;
	int c;

	if (wd_size)
		buf_len = wd_size;
	else
		buf_len = BUF_SIZE_GRAIN;

	if (f == NULL || feof(f))
		return NULL;
	assert((buf = malloc(buf_len)) != NULL);

	while (!feof(f)) {
		if (len == buf_len-1) {
			if (wd_size != 0)
				break;
			else {
				buf_len += BUF_SIZE_GRAIN;
				assert((buf = realloc(buf, buf_len)) != NULL);
			}
		}
		c = fgetc(f);
		if (c == ' ' || c == '\t' || c == '\n')
			break;
		else
			buf[len++] = c;
	}

	buf[len] = '\0';
	return buf;
}

int main()
{
	char *w;

	while (w = getword(stdin, 0)) {
		size_t len = strlen(w);
		fprintf(stdout, "Word (%d): \"%s\"\n", len, w);
		free(w);
	}

	return 0;
}

