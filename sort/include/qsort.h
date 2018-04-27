
#ifndef	_QSORT_H
#define	_QSORT_H

typedef int (*cmpFun)(const char *, const char *);

void quicksort(char *argv[], int left, int right, cmpFun cmp);

#endif	/* _QSORT_H */

