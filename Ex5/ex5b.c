/* Ex #5b: two childs make array and fill the array with random numbers % 100 ,
 * father send to each child random number and check if the random number 
 * appears in the array , until one array of the child's got coverd then 
 * we print the array of the win child that got covered frist
 *(using shared memory).
 *=============================================================================
 * Written by
 *Muhammad Egbariya - 305140931 & Mohamad Amer - 315336115
 * login
 * muhammadeg / mohamadam
 * Course/Year
 * OS1 2019
 * Year
 * B
 *=============================================================================
 * Input: Nothing.
 * ----------------------------------------------------------------------------
 * Output: print first covered array .
 *
 */
//---------------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
//---------------------------------------------------------------------------//
int FLAG = 1;

//---------------------------------------------------------------------------//
void do_child();
void print(int arr[10]);
void catchSig(int sigNum);
int bingo(int arr[10]);

//---------------------------------------------------------------------------//
int main() {
	pid_t status;	// process status
	key_t key;		// external key
	int i, 			// index
		num, 		//
		*ptr, 		// shared memory pointer
		shm_id;		// shared memory id

	// signal handler
	signal(SIGUSR1, catchSig);
	signal(SIGUSR2, catchSig);

	// ftok
	if ((key = ftok(".", 'z')) == -1) {
		perror("cannot ftok()!");
		exit(EXIT_FAILURE);
	}
	// shmget
	if ((shm_id = shmget(key, 4 * sizeof(int), IPC_CREAT | IPC_EXCL | 0600))
			== -1) {
		perror("cannot shmget()!");
		exit(EXIT_FAILURE);
	}
	// shmat
	if ((ptr = (int*) shmat(shm_id, NULL, 0)) < 0) {
		perror("cannot shmat()!");
		exit(EXIT_FAILURE);
	}

	// fork
	for (i = 0; i < 2; i++) {
		if ((status = fork()) == -1) {
			perror("cannot fork()!");
			exit(EXIT_FAILURE);
		}
		if (status == 0) {
			ptr[i] = getpid();
			do_child();
			exit(EXIT_SUCCESS);
		}
	}

	// initialize random number generator
	srand((unsigned) time(NULL));

	// raffle
	while (FLAG) {
		// waiting for both children to read number
		while (ptr[2] != 0 || ptr[3] != 0) {
			if (FLAG == 0)
				break;
		}

		// pick random number
		if (FLAG) {
			num = rand() % 100;
			ptr[2] = num;
			ptr[3] = num;
		}
	}

	// free pointer
	shmdt(ptr);

	// free shared memory
	if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
		perror("cannot shmctl()!");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
//---------------------------------------------------------------------------//
void do_child() {
	key_t key;	// external key
	int i, j,	// index
		num,	//
		*ptr, 	// shared memory pointer
		shm_id, // shared memory id
		arr1[10], 			// hold random numbers of child
		arr2[10] = { 0 };	// boolean array

	// ftok
	if ((key = ftok(".", 'z')) == -1) {
		perror("cannot ftok()!");
		exit(EXIT_FAILURE);
	}
	// shmget
	if ((shm_id = shmget(key, 0, 0600)) == -1) {
		perror("cannot shmget()!");
		exit(EXIT_FAILURE);
	}
	// shmat
	if ((ptr = (int*) shmat(shm_id, NULL, 0)) < 0) {
		perror("cannot shmat()!");
		exit(EXIT_FAILURE);
	}

	// initialize random number generator
	srand((unsigned) time(NULL) + getpid());

	// fill array with random numbers
	for (i = 0; i < 10; i++)
		arr1[i] = rand() % 100;

	// iterate relevant cell to read data
	if (getpid() == ptr[0])
		j = 2;
	else
		j = 3;

	while (1) {
		// waiting for father to pick number
		while (ptr[2] == 0 && ptr[3] == 0) {
			//sleep(1); // makes the program slow !!!
		}
		// mark as read
		num = ptr[j];
		ptr[j] = 0;

		// lose! - terminate
		if (num == -1) {
			shmdt(ptr);
			exit(EXIT_SUCCESS);
		}

		// search given number
		for (i = 0; i < 10; i++)
			if (arr1[i] == num)
				arr2[i] = 1;	// mark as found

		// covered all numbers
		if (bingo(arr2)) {
			print(arr1);
			kill(getppid(), SIGUSR1);
			if (j == 2)
				ptr[3] = -1;
			else
				ptr[2] = -1;
			shmdt(ptr);
			exit(EXIT_SUCCESS);

		}
	}
}
//---------------------------------------------------------------------------//
int bingo(int arr[10]) {
	int i, num = 1;
	for (i = 0; i < 10; i++)
		num *= arr[i];
	return num;
}
//---------------------------------------------------------------------------//
void print(int arr[10]) {
	int i;
	for (i = 0; i < 10; i++)
		printf("%d ", arr[i]);
	puts("");
}
//---------------------------------------------------------------------------//
void catchSig(int sigNum) {
	FLAG = 0;
	return;
}
//---------------------------------------------------------------------------//
