/**
 *  ex2-2: `write a loop equivalent to the for loop above without
 *          using && or ||'
 */
getline()
{
	/* for (i=0; i<lim-1 && (c=getchar()) != '\n' && c != EOF; ++i)
		s[i] = c;
	*/

	/* pseudo-code */
	for (i=0; i<lim-1; ++i) {
		if ((c = getchar()) == '\n')
			break;
		if (c == EOF)
			break;
		s[i] = c;
	}
}

