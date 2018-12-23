/* Ex #4a1: get message with two numbers, and find the GCD of the two numbers
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
int gcd(int num1,int num2);
// ============================== Globals ================================== //
int end = 0;
int msqid;
// ================================ Main =================================== //
int main() {

	//variables
	struct msgbuf1 msg1;
	int  num1, num2;
	key_t key;

	//set signal  handler
	signal(SIGINT, catch_intr);

	// start a message
	open_queue(&key, &msqid, 'g');

	while (1) {

		//receive message from ex4a3
		if (msgrcv(msqid, &msg1, sizeof(struct Data), 1, 0) == -1) {
			perror("msgrcv failed");
			end = 1;
		}
		num1 = msg1._data._num1;
		num2 = msg1._data._num2;
		msg1._data._ans=gcd(num1,num2);
		msg1.mtype = msg1._data._id;

		// send the gcd number to ex4a3
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
// This function get a key, a message queue id and a char
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
// find the gcd num of the numbers
int gcd(int num1,int num2)
{	int i,num;

		for (i = 1; i <= num1 && i <= num2; ++i) {
			if (num1 % i == 0 && num2 % i == 0) {
				num = i;
			}
		}
		return num;
}
