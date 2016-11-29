
#include <stdio.h>
#include <stdint.h>	/* C99: size_t */
#include <stdlib.h>	/* malloc */
#include <string.h>
#include <ctype.h>
#include <assert.h>


#define	BUF_SIZE_GRAIN	32


/**
 *  Input a line up to wd_size chars, including the terminating NUL
 *  If wd_size is 0, storage is allocated for the line as needed.
 *  The caller should handle cleanup
 *  The returned line can be one of:
 *  	(1) a sequence of characters making up a line without the final newline
 *  	(2) a sequence of one or more newlines
 *  Return value is 0 for EOF or error, positive value for length of
 *  a line, otherwise a negative value for how many consecutive newlines read
 */
ssize_t read_line(FILE *f, char **wptr, const size_t wd_size)
{
	size_t len = 0, buf_len;
	char *buf;
	int c, in_line = 0;

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
		if ((c = fgetc(f)) == '\n') {
			if (in_line) {
				ungetc(c, f);
				break;
			} else /* append to newline sequence 'line' */
				buf[len++] = c;
		} else {
			if (!in_line) {
				if (len > 0) { /* end of newline sequence */
					ungetc(c, f);
					break;
				} else
					in_line = 1;
			}
			buf[len++] = c;
		}
	}

	buf[len] = '\0';
	return in_line ? len : -len;
}

/**
 *   pointer to next blank, EOL if none */
const char *get_next_blank(const char *str)
{
	assert(str != NULL);
	while (*str)
		if (isblank(*str))
			return str;
		else
			str++;
	return str;
}


/**
 *  Returns pointer @position of NUL char */
const char *get_eol(const char *str)
{
	while (*str)
		str++;
	return str;
}


/**
 * Print as-is, blanks and detabbing are handled in the main routine */
size_t print_substring(const char *from, const char *to)
{
	size_t len=0;
	assert(from && to && from <= to);
	for (len=0; from <= to; len++)
		fputc(*from++, stdout);
	return len;
}

#define	TABSTOP	8
#define	MIN(a,b) ( (a) < (b) ? (a) : (b) )

void fold_line(const char *line, int COLUMNS)
{
	const char *ptr, *eol;
	int cur_col = 1;

	assert(line != NULL);
	ptr = line;
	eol = get_eol(line);

	while (ptr < eol) {
		const char *next_blank;

		next_blank = get_next_blank(ptr);
		if (next_blank == ptr) { /* code path for blanks */
			int padding = *next_blank == '\t' ?
				TABSTOP-cur_col%TABSTOP+1 : 1;
			for (; padding--; cur_col++)
				if (cur_col <= COLUMNS)
					putchar(' ');
			ptr++;
			continue;
		} /* else { */

		if (cur_col > COLUMNS) {
			/* wrap around after trailing blanks */
			fputc('\n', stdout);
			cur_col = 1;
		}
		if (cur_col + (next_blank-ptr) - 1 <= COLUMNS) {
			cur_col += print_substring(ptr, next_blank-1);
			assert(cur_col <= COLUMNS+1);
			if (cur_col == COLUMNS+1) {
				fputc('\n', stdout);
				cur_col = 1;
			}
			ptr = next_blank;
		} else while (ptr < next_blank) {
			/* split */
			if (cur_col != 1) {
				fputc('\n', stdout);
				cur_col = 1;
			}
			cur_col += print_substring(ptr, MIN(next_blank-1, ptr+COLUMNS-1));
			assert(cur_col <= COLUMNS+1);
			if (cur_col == COLUMNS+1) {
				fputc('\n', stdout);
				cur_col = 1;
			}
			ptr = MIN(next_blank, ptr+COLUMNS);
		}
		/* } // else */
	}
}

int main()
{
	char *line = NULL;
	ssize_t len;

	while (len = read_line(stdin, &line, 0)) {
		if (len > 0)
			fold_line(line, 10);
		else
			/* reduce consecutive blank lines into one */
			fputc('\n', stdout);
		if (line)
			free(line);
	}

	return 0;
}

