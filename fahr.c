/** fahr.c
 */

#include <stdio.h>

main()
{
	float fahr, celsius;

	const float lower = 0.0,
	      upper = 300.0,
	      step = 20.0;

	/* Header */
	fprintf(stdout, "Fahr\tCelsius\n");

	fahr = lower;
	while (fahr <= upper) {
		celsius = (5.0/9.0) * (fahr-32.0);
		fprintf(stdout, "%3.0f\t%6.1f\n", fahr, celsius);
		fahr += step;
	}

	fprintf(stdout, "\nCels\tFahr\n");
	celsius = lower;
	while (celsius <= upper) {
		fahr = celsius / 5.0 * 9.0 + 32.0;
		fprintf(stdout, "%3.0f\t%3.0f\n", celsius, fahr);
		celsius += step;
	}

}

