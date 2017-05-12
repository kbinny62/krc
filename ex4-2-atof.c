/**
 * ex4.2: atof() parses a floating point number in the format defined as:
 * (+|-)?[[:digit:]]*(\.[[:digit:]]*)?((E|e)(+|-)?[[:digit:]]+)?
 */
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

#define	MKFLOAT(sign, intg, frac, esign, exp) ((sign)*(intg+frac)*pow(10, esign*exp))


/**
 * TODO: for a more `freestanding' implementation, eliminate call to libc's pow()
 *       and depend on float.h alone
 */
double atof(const char *s)
{
	signed n_sign = +1, e_sign = +1;
	double intg, frac, exp;

	assert (s != NULL);

	/* strip leading blanks and extract sign, if any */
	while (isblank(*s)) s++;
	if (*s == '-' || *s == '+')
		n_sign = (*s == '-' ? -1 : +1), s++;

	
	intg = frac = exp = 0.0;	
	if (!(isdigit(*s) || *s == '.'))
		goto finalize_atof;

	/* extract integer part */
	if (*s == '.' && !isdigit(*(s+1))) 
		goto finalize_atof;
	else  for (; isdigit(*s); s++)
		intg = intg*10 + (*s-'0');

	/* extract fractional part */
	if (*s == '.') {
		int frac_factor;
		if (!isdigit(*(s+1)))
			goto finalize_atof;
		else s++;
		for (frac_factor=-1; isdigit(*s); s++)
			frac += (*s-'0') * pow(10.0, frac_factor--);
	}

	if (*s == 'e' || *s == 'E') {
		s++;
		if (*s == '+' || *s == '-')
			e_sign = (*s == '-' ? -1 : +1), s++;
		for ( ; isdigit(*s); s++)
			exp = exp*10 + (*s - '0');
	}

finalize_atof:
	return MKFLOAT(n_sign, intg, frac, e_sign, exp);
}


int main()
{
	char buf[BUFSIZ];
	while (fgets(buf, BUFSIZ, stdin))
		fprintf(stdout, "%g, %.*f\n", atof(buf),
				DBL_DIG, atof(buf));
}
