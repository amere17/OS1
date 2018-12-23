/* Ex #4a2: get message with number, and find factor primes
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdbool.h>

// ============================== Defines ================================== //
#define MAX_LEN 30
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
void open_queue(key_t* _key, int* _msqid, char _type);
void catch_intr(int s);
// ============================== Globals ================================== //
int end = 0;
int msqid;
// ================================ Main =================================== //
int main() {

	//variables
	struct msgbuf1 msg1;
	int i, j, size, num1, PrimeNum;
	key_t key;
	//set signal  handler
	signal(SIGINT, catch_intr);
	// start a message
	open_queue(&key, &msqid, 'd');

	while (1) {
		size = 0;
		//receive message from ex4a3
		if (msgrcv(msqid, &msg1, sizeof(struct Data), 1, 0) == -1) {
			perror("msgrcv failed");
			end = 1;
		}
		num1 = msg1._data._num1;

		for (i = 2; i <= num1; i++) {
			/* Check 'i' for factor of num */
			if (num1 % i == 0) {
				/* Check 'i' for Prime */
				PrimeNum = 1;
				for (j = 2; j <= i / 2; j++) {
					if (i % j == 0) {
						PrimeNum = 0;
						break;
					}
				}

				/* If 'i' is Prime number and factor of num */
				if (PrimeNum == 1) {
					msg1._data._num[size] = i;
					size++;
				}
			}

		}
		msg1._data._num1 = size;
		msg1.mtype = msg1._data._id;

		if (!end && msgsnd(msqid, &msg1, sizeof(struct Data), 0) == -1) {
			perror("msgsnd failed");
			exit(EXIT_FAILURE);
		}
		if (end == 1) {
			if (msgctl(msqid, IPC_RMID, NULL) == -1) {
				perror("msgctl failed");
				exit(EXIT_FAILURE);
			}
			break;
		}
	}
	return EXIT_SUCCESS;
}

// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
// Checks if can open the queue using the char as a 'type'
void open_queue(key_t* _key, int* _msqid, char _type) {
	if (((*_key) = ftok("/.", _type)) == -1) {
		perror("ftok failed");
		exit(EXIT_FAILURE);
	}

	if (((*_msqid) = msgget(*_key, 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
		perror("msgget failed");
		exit(EXIT_FAILURE);
	}
}
//-----------------------------------------------------------------------------
void catch_intr(int s) {
	end = 1;
}
//-----------------------------------------------------------------------------
