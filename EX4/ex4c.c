/* Ex #4c: make two sons then give array of random numbers between 0-100 to
 * the two sons and send from the parent random number then check if the number
 * is found in arrays , and find which array get covered first ,
 * print the array then send to stop the lose son .
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
 * Output: Array of the child who wins
 *
 */
// ============================== Include ================================== //
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include <time.h>

//============================== defines ================================== //
#define ARR_LEN 10

//============================== struct ================================== //
struct Data {
	pid_t _id;
	int num;

};
struct msgbuf {
	long mtype;
	struct Data _data;

};
//============================== prototype ================================== //
int msqid, end = 0, Childs = 2;
void open_queue(key_t* _key, int* _msqid, char _type);
void sendMsg(struct msgbuf msg);
void doSon(int Son);
void check_fork(pid_t status);
void doFather(pid_t child_A, pid_t child_B);
int arr_done(int arr[ARR_LEN]);
void checkArr(int arr[ARR_LEN], int flag[ARR_LEN], int num);
void print_arr(int arr[ARR_LEN]);
void check_Child(pid_t child_A, pid_t child_B);

//============================== Main ================================== //
int main() {
	//==================== Variables ============================
	pid_t child_A, child_B;
	key_t key;
	open_queue(&key, &msqid, 'c');
	child_A = fork();
	check_fork(child_A);
	srand(time(NULL));

	if (child_A == 0) {
		doSon(1);
	} else {
		child_B = fork();
		check_fork(child_B);
		if (child_B == 0) {
			doSon(2);
		} else {
			doFather(child_A, child_B);
		}
	}

	return EXIT_SUCCESS;
}

//================================================================ //
// do father function, send and get message from the son
void doFather(pid_t child_A, pid_t child_B) {
	int num;
	struct msgbuf msg;
	srand(time(NULL));
	while (1) {
		num = (rand() % 100);
		msg._data.num = num;
		msg.mtype = child_A;
		sendMsg(msg);
		msg.mtype = child_B;
		sendMsg(msg);

		check_Child(child_A, child_B);
		check_Child(child_A, child_B);


	}

}
//========================================================================== //
//do son function and check if the array covered send message to father to end
void doSon(int Son) {
//==================== Variables ============================
	int i, arr[ARR_LEN], flag[ARR_LEN] = { 0 };
	struct msgbuf msg, dad;
	srand(time(NULL) + Son);

//================= get random numbers ===================
	for (i = 0; i < ARR_LEN; i++)
		arr[i] = (rand() % 100);

	while (1) {
		//receive message from dad
		if (msgrcv(msqid, &dad, sizeof(struct Data), getpid(), 0) == -1) {
			perror("msgrcv failed");
			exit(EXIT_FAILURE);
		}

		if (dad._data.num == -1) {
			msg.mtype = 1;
			msg._data.num = -2;
			sendMsg(msg);
			exit(EXIT_SUCCESS);
		}

		msg.mtype = 1;
		checkArr(arr, flag, dad._data.num);
		if (arr_done(flag) == 1) {
			msg._data.num = -1;
			sendMsg(msg);
			print_arr(arr);
			exit(EXIT_SUCCESS);

		}

		msg._data.num = dad._data.num;
		sendMsg(msg);
	}

}
//===================== check child win =============================
void check_Child(pid_t child_A, pid_t child_B) {
	struct msgbuf dad, msg1;
	int num;

	if (msgrcv(msqid, &dad, sizeof(struct Data), 1, 0) == -1) {
		perror("msgrcv failed");
		exit(EXIT_FAILURE);
	}

	num = dad._data.num;
// check which son is win and stop the lose son
	if (num == -1) {
		msg1._data.num = -1;
		if (dad._data._id == child_A)
			msg1.mtype = child_B;
		else
			msg1.mtype = child_A;

		sendMsg(msg1);
	} else if (num == -2) {
		if (msgctl(msqid, IPC_RMID, NULL) == -1) {
			perror("msgctl failed");
			exit(EXIT_FAILURE);
		}

		exit(EXIT_SUCCESS);
	}

}
//===================== check fork ============================================
void check_fork(pid_t status) {
	if (status == -1) {
		perror("cannot fork()");
		exit(EXIT_FAILURE);
	}
}
//===================== check array done ======================================
int arr_done(int arr[ARR_LEN]) {
	int i;
	for (i = 0; i < ARR_LEN; i++) {
		if (arr[i] == 0)
			return 0;
	}
	return 1;
}
//===================== check if number found =================================
void checkArr(int arr[ARR_LEN], int flag[ARR_LEN], int num) {

	int i;
	for (i = 0; i < ARR_LEN; i++) {
		if (arr[i] == num) {
			flag[i] = 1;
		}
	}
}
//===================== send message ==========================================
void sendMsg(struct msgbuf msg) {

	if (!end && (msgsnd(msqid, &msg, sizeof(msg._data), 0)) == -1) {
		perror("msgsnd failed");
		end = 1;
	}
}
//=========================== print array =====================================
void print_arr(int arr[ARR_LEN]) {

	int i;
	for (i = 0; i < ARR_LEN; i++)
		printf("%d ", arr[i]);
	puts("");

}
//======================= start new message ===================================
void open_queue(key_t* _key, int* _msqid, char _type) {
	if (((*_key) = ftok(".", _type)) == -1) {
		perror("ftok failed");
		exit(EXIT_FAILURE);
	}
	*_msqid = msgget(*_key, 0600 | IPC_CREAT | IPC_EXCL);
	if (*_msqid == -1) {
		perror("msgget failed");
		exit(EXIT_FAILURE);
	}

}

