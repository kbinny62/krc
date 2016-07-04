
#include <stdio.h>
#include <stdint.h>	/* C99: size_t */
#include <stdlib.h>	/* malloc */
#include <assert.h>

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

	assert((buf = malloc(buf_len)) != NULL);

	while (!feof(f)) {
		if (len == buf_len-1)
			if (wd_size != 0)
				break;
			else {
				buf_len += BUF_SIZE_GRAIN;
				assert((buf = realloc(buf_len)) != NULL);
			}
		c = getchar();
		switch (c) {
			case ' ':
			case '\t':
			case '\n':
				break;
			default:
				s[len++] = c;
				break;
	}

	s[len] = '\0';
	return len;
}

int main()
{
	unsigned int i;
	int c;

	if (maxlen == 0)
		return 0;

	i = 0;
	while ((c = getchar()) != EOF) {
		s[i++] = c;
		if (c == '\n')
			break;
	}

	s[i] = '\0';
	return i;
}

