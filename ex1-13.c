/**
 *  K&R 2: ex 1.13 (modified/enahnced)
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define	WORDSIZE	20

struct word_list {
	char word[WORDSIZE];
	int count;
	struct word_list *next;
};


void count_word(struct word_list *wlist, const char *word)
{
	assert(word != NULL);
	assert(wlist != NULL);

	while (wlist->next) {
		if (strncmp(word, wlist->next->word, WORDSIZE) == 0) {
			wlist->count++;
			return;
		} else
			wlist = wlist->next;
	}

	/* No match, append */
	wlist->next = malloc(sizeof(struct word_list));
	assert(wlist->next != NULL);
	strncpy(wlist->next->word, word, WORDSIZE);
	wlist->next->count = 1;
	wlist->next->next = NULL;
}

void sort_words(struct word_list *wlist)
{
}

/**
 *  Histograms output is scaled up to maximum value as 100%
 *  if n > 0, only the the first 'n' elements are listed
 */
#define	HISTOGRAM_UNIT	50 /* Asterisks */
void print_histogram(struct word_list *wlist, int head)
{
	struct word_list *w;
	int max = 0;
	int head_counter;

	assert(wlist != NULL);


	for (w = wlist->next, head_counter = head; w != NULL; w = w->next) {
		max = w->count > max ? w->count : max;
		if (head > 0 && --head_counter == 0)
			break;
	}

	for (w = wlist->next, head_counter = head; w != NULL; w = w->next) {
		int i;
		/* Justify */
		for (i = 0; i<WORDSIZE-1-strlen(w->word); i++)
			putchar(' ');

		fprintf(stdout, "%s: %2d: ", w->word, w->count);
		for (i = 0; i < w->count * HISTOGRAM_UNIT / max; i++)
			putchar('*');
		putchar('\n');

		if (head > 0 && --head_counter == 0)
			break;
	}
	
}


#define	HISTO_HEAD	10

int main(int argc, char *argv[])
{
	int in_word, c; /* 'parser' state */
	char word[WORDSIZE]; /* WORDSIZE chars including NUL */
	int word_idx; /* next char within 'word' */
	struct word_list wlist;

	int opt_sort = 0;
	int opt_head = 0;

	const char *exename = argv[0];

	while (--argc) {
		++argv;
		if (!strcmp(*argv, "-s"))
			opt_sort = 1;
		if (!strcmp(*argv, "-n"))
			opt_head = 1;
		if (!strcmp(*argv, "-h")) {
			fprintf(stdout, "Usage: %s [-s] [-n]\n"
					"-s to sort word counts\n"
					"-n to display maximum of %d entries\n",
					exename, HISTO_HEAD);
			return 0;
		}
	}

	memset(&wlist, 0, sizeof(wlist));
	word[word_idx = 0] = '\0';
	in_word = 0;

	while ((c = getchar()) != EOF) {

		if (c == ' ' || c == '\t' || c == '\n') {
			if (in_word) {
				assert(word_idx < WORDSIZE);
				word[word_idx] = '\0';
				count_word(&wlist, word);
			}
			in_word = 0;
		} else {
			if (!in_word) {
				in_word = 1;
				word[word_idx = 0] = '\0';
			}
			if (word_idx < WORDSIZE-1)
				word[word_idx++] = c;
			else { /* Silently truncate */ }
		}
	}

	if (opt_sort)
		sort_words(&wlist);

	print_histogram(&wlist, opt_head ? HISTO_HEAD : 0);

	return 0;
}

