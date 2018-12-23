/* Ex #4b3:  the client of two servers that find gcd or factor primes using
 * private servers(using fifo files )
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
 * Input: using argument.
 * ----------------------------------------------------------------------------
 * Output: if argv[1]=0 the output is gcd number.
 *         if argv[1]=1 the output is primes factor of number.
 * Example: (in terminal) we have to run the servers first
 * 		    (fifo file opens by the servers),
 * 			then ./ex4b3 0 12 14
 * Output : 2
 *
 */
// ============================== Include ================================== //
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>

// ============================== Defines ================================== //
#define FIFO_NAME1 "app_server1"
#define FIFO_NAME2 "app_server2"
#define MAX_LEN    100
// ============================== Globals ================================== //
FILE *app_w, *app_r;
// ================================ Main =================================== //
int main(int argc, char **argv) {

	char s[] = "         ";
	int num;
	char txt[MAX_LEN] = { 0 };
	num = atoi(argv[1]);
	if (num == 0)
		app_w = fopen(FIFO_NAME1, "w");
	else if (num == 1)
		app_w = fopen(FIFO_NAME2, "w");

	if (num == 0) {

		int num1, num2;
		num1 = atoi(argv[2]);
		num2 = atoi(argv[3]);
		sprintf(s, "%d", num1);
		strcpy(txt, s);
		sprintf(s, "%d", num2);
		strcat(txt, " ");
		strcat(txt, s);

	} else if (num == 1) {
		int number = atoi(argv[2]);
		sprintf(s, "%d", number);
		strcpy(txt, s);

	}
	// returns answer from named pipe
	fprintf(app_w, " %s", txt);
	fflush(app_w);
	fclose(app_w);

	if (num == 0) {
		app_r = fopen(FIFO_NAME1, "r");
		fscanf(app_r, "%s", txt);
		printf("%s\n", txt);
	}

	else if (num == 1) {
		app_r = fopen(FIFO_NAME2, "r");
		fgets(txt,MAX_LEN,app_r);
		printf("%s\n", txt);
	}
	fclose(app_r);
	return EXIT_SUCCESS;


}
