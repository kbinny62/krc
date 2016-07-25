
#include <stdio.h>
#include <stdint.h>	/* C99: size_t */
#include <stdlib.h>	/* malloc */
#include <string.h>
#include <assert.h>

#define	BUF_SIZE_GRAIN	32

const char word_delimiters[] = { ' ', '\t', '\n', '\0' };

/**
 *  Input a word up to wd_size chars, including the terminating NUL
 *  If wd_size is 0, storage is allocated for the word as needed.
 *  The caller should handle cleanup
 *  The returned word can be one of:
 *  	(1) a sequence of non-delimiter chars
 *  	(2) a sequence of delimiters
 *  Return value is 0 for EOF or error, positive value for length of
 *  a word, otherwise a negative value for length of delimiter sequence
 */
ssize_t getword(FILE *f, char **wptr, const size_t wd_size)
{
	size_t len = 0, buf_len;
	char *buf;
	int c, in_word = 0;

	if (f == NULL || feof(f) || wptr == NULL)
		return 0;

	buf_len = wd_size ? wd_size : BUF_SIZE_GRAIN;
	assert((buf = *wptr = malloc(buf_len)) != NULL);

	while (!feof(f)) {
		if (len == buf_len-1) {
			if (wd_size != 0)
				break;
			else {
				buf_len += BUF_SIZE_GRAIN;
				assert((buf = *wptr = realloc(buf, buf_len)) != NULL);
			}
		}
		c = fgetc(f);
		if (strchr(word_delimiters, c)) {
			if (in_word) {
				ungetc(c, f);
				break;
			} else /* append to delimiter 'word' */
				buf[len++] = c;
		} else {
			if (!in_word) {
				if (len > 0) { /* end of delimiter 'word' */
					ungetc(c, f);
					break;
				} else
					in_word = 1;
			}
			buf[len++] = c;
		}
	}

	buf[len] = '\0';
	return in_word ? len : -len;
}


int main()
{
	char *w = NULL;
	ssize_t len;

	while (len = getword(stdin, &w, 0)) {
		if (len > 0)
			fprintf(stdout, "Word (%zu): \"%s\"\n", len, w);
		else
			fprintf(stdout, "Blank (%zu)\n", -len);
		if (w)
			free(w);
	}

	return 0;
}

