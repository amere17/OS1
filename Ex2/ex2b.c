/* Ex #2b: the father process do child, each process enter to infinity loop
 * 			then make a random choose with modulo 7,
 * 			num=0 : process done .
 * 			num=1,2,3: send signal to another process.
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
 * Input:
 * ----------------------------------------------------------------------------
 * Output:  process 110996 has a partner
			process 110996 has a partner
			process 110996 was left alone, and quits
			process 111001 was left alone, and quits.
 **************************/

//---------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for fork()
#include <signal.h>
#include <sys/types.h>
#include <time.h>

//---------------------------------

void catch_sigusr(int sig_turn);
void catch_alarm(int sig_turn);
void doFather(pid_t pid);
void checkSon();
void checkFather();
void checFork(pid_t status);
//---------------------------------------------------------
int father = 0, son = 0;
pid_t dad;
//---------------------------------------------------------
int main() {
	pid_t status;

	signal(SIGUSR1, catch_sigusr); // definde signal hendlers
	signal(SIGALRM, catch_alarm);

	dad = getpid();
	status = fork();
	checFork(status);
	
	while (1) {
		if (status < 0) {
			printf("process %d end\n", getpid());
			exit(EXIT_SUCCESS);
		} else {
			srand(time(NULL));
			if (status == 0) {
				sleep(2);

				for (;;) {
					

					int num = rand() % 7;
					if (num == 0) {
						printf("process %d ends\n", getpid());
						exit(EXIT_SUCCESS);
					} else if (num >= 1 && num <= 3) {
						kill(getppid(), SIGUSR1);
						alarm(5);
						pause();
					}
				}
			} else
				doFather(dad);

		}
	}

	return EXIT_SUCCESS;
}

//---------------------------------------------------------

void doFather(pid_t pid) {

	for (;;) {

		int turn = rand() % 7;

		if (turn == 0) {
			printf("process %d ends\n", getpid());
			exit(EXIT_SUCCESS);
		}

		else if (turn >= 1 && turn <= 3) {
			alarm(5);
			pause();
			kill(pid, SIGUSR1);
		}
	}

}
//---------------------------------------------------------
//---------------------------------------------------------
void checFork(pid_t status) {
	if (status < 0) {
		fputs("Cannot fork()", stderr);
		exit(EXIT_FAILURE);
	}
}
//---------------------------------------------------------

void catch_alarm(int sig_turn) {
	signal(SIGALRM, catch_alarm);
	alarm(0);
	printf("process %d was left alone, and quits\n", getpid());
	exit(EXIT_SUCCESS);
}
//---------------------------------------------------------
void catch_sigusr(int sig_turn) {

	signal(SIGUSR1, catch_sigusr);
	alarm(0);

	if (getpid() == dad)
		father++;
	else
		son++;

	if(father==10)
		checkFather();
	else if(son==10)
		checkSon();
	else
	printf("process %d has a partner\n", getpid());
}
//---------------------------------------------------------
void checkFather() {
		printf("process %d surrender\n", getpid());
		father = 0;
		exit(EXIT_SUCCESS);

}
//---------------------------------------------------------
void checkSon()
{
	printf("process %d surrender\n", getpid());
	son = 0;
	exit(EXIT_SUCCESS);
}
