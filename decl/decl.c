/*
 *  TODO:
 *  	- function argument types
 *  	- qualifiers (const, static, unsigned, etc..)
 */
#include <stdio.h>
#include <string.h>
#include "token.h"

char datatype[MAXTOKEN];        /* data type = char, int, etc. */
char name[MAXTOKEN];
char out[1000];                 /* output string */

int dcl(void);
int dirdcl(void);

int dirdcl()
{
	int type;

	if (tokentype == '(') {
		if (dcl() == 0) {
			fprintf(stderr, "dirdcl: sub-dcl() failed.\n");
			return 0;
		}
		if (tokentype != ')') {
			fprintf(stderr, "dirdcl: unmatched parens.\n");
			return 0;
		}
	} else if (tokentype == NAME) {
		strncpy(name, token, MAXTOKEN);
	} else {
		fprintf(stderr, "dirdcl: expected parens or identifier.\n");
		return 0;
	}

	while ((type = gettoken()) == PARENS || type == BRACKETS) {
		if (type == PARENS)
			strcat(out, " function returning");
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
	}

	return 1;
}

int dcl()
{
	unsigned ns = 0;
	while (gettoken() == '*')
		ns++;
	if (dirdcl() == 0) {
		fprintf(stderr, "dcl: dirdcl() failed.\n");
		return 0;
	}
	while (ns--)
		strcat(out, " pointer to");
	return 1;
}

/**
 *  convert declaration to words
 */
int main()
{
	while (gettoken() != EOF) {
		strcpy(datatype, token);
		*out = '\0';
		if (dcl() == 0 || tokentype != '\n') {
			fprintf(stderr, "main: syntax error (token: '%s', tokentype: %i, name: '%s', out: '%s', datatype: '%s')\n", token, tokentype, name, out, datatype);
			/* skip remainder of line */
			while (gettoken() != EOF)
				if (tokentype == '\n')
					break;
			if (tokentype == EOF)
				break;
		} else
			printf("%s %s %s\n", name, out, datatype);
	}
	return 0;
}
