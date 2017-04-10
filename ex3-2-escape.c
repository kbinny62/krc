
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void escape(char *dst, const char *src)
{
	char c;
	while (c = *src++) {
		switch (c) {
		case '\n':
			*dst++ = '\\';
			*dst++ = 'n';
			break;
		case '\t':
			*dst++ = '\\';
			*dst++ = 't';
			break;
		default:
			*dst++ = c;
			break;
		}
	}

	*dst = '\0';
}

void unescape(char *dst, const char *src)
{
	while (*src) {
		if (*src == '\\')
			switch (*++src) {
			case 'n':
				*dst++ = '\n';
				src++;
				break;
			case 't':
				*dst++ = '\t';
				src++;
				break;
			default:
				*dst++ = *src++;
			}
		else
			*dst++ = *src++;
	}
	*dst = '\0';
}


#define	MAX_BUFSZ	1024

int main(int argc, char *argv[])
{
	char *buf, *buf2;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return EXIT_FAILURE;
	} else {
		FILE *fp = fopen(argv[1], "r");
		size_t sz;

		assert(fp != NULL);
		fseek(fp, 0, SEEK_END);
		sz = ftell(fp) > MAX_BUFSZ ? MAX_BUFSZ : ftell(fp);
		assert((buf = (char*)malloc(sz+1)) != NULL);
		assert((buf2 = (char*)malloc(sz*2+1)) != NULL);

		fseek(fp, 0, SEEK_SET);
		fread(buf, sz, 1, fp);
		buf[sz] = '\0';
		escape(buf2, buf);
		fprintf(stdout, "Escaped text: '%s'\n", buf2);
		unescape(buf, buf2);
		fprintf(stdout, "\nOriginal:\n%s\n", buf);
		fclose(fp);
	}

	return 0;
}

