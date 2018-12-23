/* Ex #4b1:find the GCD of the two numbers and
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
#define FIFO_NAME1 "app_server1"
#define MAX_LEN    100

// ============================= Prototypes ================================ //
void catch_intr(int s);
int gcd(int a, int b);
// ================================ Main =================================== //
int main() {
	// Variables
	FILE *client_r;
	char txt[MAX_LEN]={0};
	int ans, a, b;

	// mkfifo
	mkfifo(FIFO_NAME1, S_IFIFO | 0644);

	//check signal
	signal(SIGINT, catch_intr);

	while (1) {
		// open file to read data through fifo file
		client_r = fopen(FIFO_NAME1, "r");
		if (!client_r) {
			perror("cannot open fifo file for r");
			exit(EXIT_FAILURE);
		}

		fscanf(client_r, "%d %d", &a, &b);
		ans = gcd(a, b);
		sprintf(txt,"%d",ans);
		fclose(client_r);
		//open fifo file to write data
		if (!(client_r = fopen(FIFO_NAME1, "w"))) {
			perror("cannot open fifo file for w");
			exit(EXIT_FAILURE);
		}

		fprintf(client_r, "%s\n", txt);
		fflush(client_r);
		fclose(client_r);
	}
	return EXIT_SUCCESS;
}

// ============================= Functions ================================= //
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
// find gcd of two numbers
int gcd(int a, int b) {
	if (a == 0)
		return b;
	return gcd(b % a, a);
}
