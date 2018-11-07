/* EX #1b4: MAX.
 * =============================================================================
 * Written by
 : Muhammad Egbariya - 305140931 & Mohamad Amer - 315336115
 * login
 : muhammadeg / mohamadam
 * Course/Year
 : OS1 2019
 * Year
 : B
 * =============================================================================
 * Input: strings through the argv.
 * -----------------------------------------------------------------------------
 * Output: max number.
 ******************************************************************************/

//---------------------- include section --------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------------- main -------------------------------------------------
int main(int argc, char** argv) {
	int max, i;
	if (strspn(argv[0], "-0123456789.") == strlen(argv[0])) 
		max = atoi(argv[0]);
	else
	{
		puts("error");
		exit(EXIT_FAILURE);
	}
	for (i = 1; i < argc; i++) {
		if (strspn(argv[i], "-0123456789.") == strlen(argv[i])) {
			if (atoi(argv[i]) > max)
				max = atoi(argv[i]);
		} else {
			puts("error");
			exit(EXIT_FAILURE);
		}
	}
	printf("%d", max);

	exit(EXIT_SUCCESS);
}
//-----------------------------------------------------------------------------
