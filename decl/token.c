#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "token.h"

int tokentype;
char token[MAXTOKEN];

/**
 *  From K&R2: $5.12, p. 125:
 *  The function gettoken skips blanks and tabs, then find the next token in
 *  the input; a "token" is a name, a pair of parentheses, a pair of brackets
 *  perhaps including a number, or any other single character.
 *
 *  Return value, token/tokentype state by input:
 *  	- Parens '([::blank::]*)':
 *  	  return: PARENS, token: "()", tokentype: PARENS
 *  	- LParen '(':
 *  	  return: '(', token: UNSPECIFIED, tokentype: '('
 *  	- Brackets '[<content>]':
 *  	  return: BRACKETS, token: '[<content>]', tokentype: BRACKETS
 *  	- [::alpha::][::alnum::]*:
 *  	   
 */
int gettoken(void)
{
	int c;

	while ((c = getchar()) == ' ' || c == '\t')
		;
	if (c == '(') {
		while ((c = getchar()) == ' ' || c == '\t')
			;
		if (c == ')') {
			strcpy(token, "()");
			return tokentype = PARENS;
		} else {
			ungetc(c, stdin);
			return tokentype = '(';
		}
	} else if (c == '[') {
		char *p = token;
		for (*p++ = c; (*p++ = getchar()) != ']'; )
			if (p >= token + MAXTOKEN - 1)
				break;
		*p = '\0';
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		char *p = token;
		for (*p++ = c; ; ) {
			c = getchar();
			if (!isalnum(c) || p >= token + MAXTOKEN - 1)
				break;
			else
				*p++ = c;
		}
		*p = '\0';
		ungetc(c, stdin);
		return tokentype = NAME;
	} else
		return tokentype = c;
}
