 /* Ex #1b2:MINUS 
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
* Input: argv with two numbers.
* -----------------------------------------------------------------------------
* output: difference of two numbers.
*
******************************************************************************/

//-------------------------- include section ----------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------------- main --------------------------------------------- 
int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "%s Usage: <input file> \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if(strspn(argv[1], "-0123456789.") == strlen(argv[1])
				&& strspn(argv[0], "-0123456789.") == strlen(argv[0])){
		int num = (atoi(argv[0]) - atoi(argv[1]));
		printf("%d", num);
	} else {
		puts("error");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
//-----------------------------------------------------------------------------
