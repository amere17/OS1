/* Ex #4a3: the client of two servers that find gcd or factor primes using
 * private servers(using messages)
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
 * Example: (in terminal) we have to run the servers first,
 * 			then ./ex4b3 0 12 14
 * Output : 2
 *
 */
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

// ============================== Defines ================================== //
#define MAX_LEN 30                  // size of text message

// ============================== Structs ================================== //
struct Data {
	pid_t _id;
	int _num1, _num2;
	int _num[MAX_LEN];
	int _ans;
};

struct msgbuf1 {
	long mtype;
	struct Data _data;

};

// ============================= Prototypes ================================ //
void connect_queue(key_t* _key, int* _msqid, char _type);
// ================================ Main =================================== //
int main(int argc, char **argv) {

	//variables
	int msqid1, msqid2;
	int ans, i;
	key_t key1, key2;
	struct msgbuf1 msg1;

	//check if the argc is good
	if (argc != 3 && argc != 4) {
		puts("Input error");
		exit(EXIT_FAILURE);
	}

	msg1.mtype = 1;

	ans = atoi(argv[1]);
	if (ans == 0) {
		connect_queue(&key1, &msqid1, 'g');
		msg1._data._num1 = atoi(argv[2]);
		msg1._data._num2 = atoi(argv[3]);
		msg1._data._id = getpid();

		if (msgsnd(msqid1, &msg1, sizeof(struct Data), 0) == -1) {
			perror("msgsnd failed");
			exit(EXIT_FAILURE);
		}
		if (msgrcv(msqid1, &msg1, sizeof(struct Data), (int) getpid(), 0)
				== -1) {
			perror("msgrcv failed");
			exit(EXIT_FAILURE);
		}

		printf("%d\n", msg1._data._ans);
	} else if (ans == 1) {
		connect_queue(&key2, &msqid2, 'd');
		msg1._data._num1 = atoi(argv[2]);
		msg1._data._id = getpid();
		if (msgsnd(msqid2, &msg1, sizeof(struct Data), 0) == -1) {
			perror("msgsnd failed");
			exit(EXIT_FAILURE);
		}
		if (msgrcv(msqid2, &msg1, sizeof(struct Data), (int) getpid(), 0)
				== -1) {
			perror("msgrcv failed");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < msg1._data._num1; i++) {
			printf("%d ", msg1._data._num[i]);
		}
		printf("\n");
	}

	return EXIT_SUCCESS;

}
//---------------------------------------------------------------------------//
// Checks if can open connect the queue using the char as a 'type'
void connect_queue(key_t* _key, int* _msqid, char _type) {
	if (((*_key) = ftok("/.", _type)) == -1) {
		perror("ftok failed");
		exit(EXIT_FAILURE);
	}

	// connect to an exist queue

	if (((*_msqid) = msgget((*_key), 0)) == -1) {
		perror("msgget failed");
		exit(EXIT_FAILURE);
	}
}
//---------------------------------------------------------------------------//

