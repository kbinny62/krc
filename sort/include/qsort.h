
#ifndef	_QSORT_H
#define	_QSORT_H

typedef int (*cmpFun)(char *, char *);

void qsort(char *argv[], int left, int right, cmpFun cmp);

#endif	/* _QSORT_H */

