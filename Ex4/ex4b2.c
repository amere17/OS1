/* Ex #4b2:get number, to find factor primes of it and
 * send the answer using the fifo files
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
 * Input: Nothing.
 * ----------------------------------------------------------------------------
 * Output: Nothing.
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
#include <signal.h>
#include <stdbool.h>

// ============================== Defines ================================== //
#define FIFO_NAME1 "app_server2"
#define MAX_LEN    100

// ============================= Prototypes ================================ //
void catch_intr(int s);
void prime(int number);
FILE *client_r;
char txt[MAX_LEN];

// ================================ Main =================================== //
int main() {	// Variables

	int a;
	// mkfifo
	mkfifo(FIFO_NAME1, S_IFIFO | 0644);

	//check signal
	signal(SIGINT, catch_intr);

	while (1) {
		client_r = fopen(FIFO_NAME1, "r");
		if (!client_r) {
			perror("cannot open fifo file for r");
			exit(EXIT_FAILURE);
		}

		fscanf(client_r, "%d", &a);
		fclose(client_r);
		prime(a);
		if (!(client_r = fopen(FIFO_NAME1, "w"))) {
			perror("cannot open fifo file for w");
			exit(EXIT_FAILURE);
		}
		fprintf(client_r,txt);
		fflush(client_r);
		fclose(client_r);

	}
	return EXIT_SUCCESS;
}

// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
// Signal handler for SIGINT
void catch_intr(int s) {
	char str[MAX_LEN];

	if (s == SIGINT) {
		sprintf(str, "rm %s", FIFO_NAME1);
		system(str);
	}

	exit(EXIT_SUCCESS);
}
//---------------------------------------------------------------------------//
// find primes factor of number
void prime(int number) {
	char s[MAX_LEN];
	int d = 2;
	int count = 0;
	while (number != 0) {
		if (number % d != 0)
			d = d + 1;
		else {
			number = number / d;
			sprintf(s, "%d", d);

			if (count == 0) {
				strcpy(txt, s);
				count = 1;
			} else
				strcat(txt, s);
			strcat(txt, " ");
			if (number == 1)
				break;
		}

	}
	
}
