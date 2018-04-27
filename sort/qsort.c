
#include "qsort.h"

static void swap(char *v[], int i, int j) {
	char *tmp = v[i];
	v[i] = v[j];
	v[j] = tmp;
}

void quicksort(char *argv[], int left, int right, cmpFun cmp)
{
	int last, i;
	if (right <= left)
		return;

	swap(argv, left, (right + left) / 2);
	last = left;
	for (i=last; i <= right; i++)
		if (cmp(argv[i], argv[left]) < 0)
			swap(argv, ++last, i);
	swap(argv, left, last);

	quicksort(argv, left, last-1, cmp);
	quicksort(argv, last+1, right, cmp);
}

