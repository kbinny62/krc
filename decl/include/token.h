
#ifndef	_TOKEN_H
#define	_TOKEN_H

enum /* token_type */  {
	NAME,
	PARENS,
	BRACKETS
};

extern int gettoken(void);

#define	MAXTOKEN	100

extern int tokentype;           /* type of last token */
extern char token[MAXTOKEN];    /* last token string */
extern char name[MAXTOKEN];     /* identifier name */

#endif	/* _TOKEN_H */
