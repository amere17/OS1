/* EX #1b3: SUM
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
 * Output: the sum of all the numbers
 ******************************************************************************/

//----------------------- include section -------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------------------- main ---------------------------------------
int main(int argc, char** argv) {
	int sum = 0, i;
	for (i = 0; i < argc; i++) {
		if (strspn(argv[i], "-0123456789.") == strlen(argv[i]))
			sum += atoi(argv[i]);
		else
		{
			puts("error");
			exit(EXIT_FAILURE);
		}
	}
	printf("%d", sum);
	exit(EXIT_SUCCESS);
}
//-----------------------------------------------------------------------------
