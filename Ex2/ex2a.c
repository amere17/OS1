/* Ex #2a: make a challenge between two processes .
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
 * Input: .
 * ----------------------------------------------------------------------------
 * Output: process <114278> got signal SIGUSR1.
	X=0 - Y=0
	process <114283> got signal SIGUSR1.
	X=1 - Y=0
	process <114278> got signal SIGUSR1.
	X=1 - Y=0
	process <114283> got signal SIGUSR1.
	X=1 - Y=1
	process <114278> got signal SIGUSR1.
	X=1 - Y=1
	process <114283> win 2 1.
	process <114278> win 1 1..
 **************************/

//--------------------------- include section ---------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
//-------------------------- define section -----------------------------------
// global counters
int X = 0, Y = 0;

//------------------------ prototypes section----------------------------------
void doChild(int num);
void doParent(pid_t status, int num);
void catchSigusr1(int sigNum);
void catchTerm(int sigNum);

//-------------------------- main ---------------------------------------------
int main() {
	int i, num;
	pid_t status;

	// set signal handlers
	signal(SIGUSR1, catchSigusr1);
	signal(SIGTERM, catchTerm);

	// initialize random number generator
	srand((unsigned) time(NULL));

	// check fork()
	status = fork();
	if (status < 0) {
		fputs("Cannot fork()", stderr);
		exit(EXIT_FAILURE);
	}
	// enter loop
	for (i = 0; i < 10; i++) {
		num = rand() % 2;
		sleep(rand() % 4);
		if (status == 0)
			doChild(num);
		else
			doParent(status, num);
	}

	return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
void doChild(int num) {
	if (num == 0)
		X++;
	else
		Y++;

	kill(getppid(), SIGUSR1);
	pause();

	if (X >= 2 && (X < Y || Y == 2 * X)) {
		printf("process <%d> surrender \n", getpid());
		kill(getppid(), SIGTERM);
		exit(EXIT_SUCCESS);
	}
}
//-----------------------------------------------------------------------------
void doParent(pid_t status, int num) {
	pause();
	if (num == 0)
		X++;
	else
		Y++;

	kill(status, SIGUSR1);

	if (Y >= 2 && (X > Y || X == 2 * Y)) {
		printf("process <%d> surrender \n", getpid());
		kill(status, SIGTERM);
		exit(EXIT_SUCCESS);
	}
}
//-----------------------------------------------------------------------------
void catchSigusr1(int sigNum) {
	//signal(SIGUSR1, catchSigusr1);
	printf("process <%d> got signal SIGUSR1.\nX=%d - Y=%d\n", getpid(), X, Y);
}
//-----------------------------------------------------------------------------
void catchTerm(int sigNum) {
	printf("process <%d> win %d %d.\n", getpid(), X, Y);
	exit(EXIT_SUCCESS);
}
//-----------------------------------------------------------------------------
