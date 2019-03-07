/* Ex #9b: find the first child thats covered from the father ( the father
 *  make radnom number  and the children check if the number is found in the
 *  arrays)
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
 * Output: the first child that covered all his array from the random numbers
 * of the father and then print the array of the covered child
 *
 *
 */
//------------- include section -------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <semaphore.h>

//------------- global section -------------------
#define SHM_SIZE 3
#define NUM_OF_ITEMS 10
#define NUM_OF_SONS 2
#define TWO_NUMS 2

//------------- Function section -------------------
int * check_fun(int *shm_ptr);
void Print(unsigned arr[NUM_OF_ITEMS]);
void open_mutex(sem_t* mutexes[]);
void close_mutex(sem_t* mutexes[]);
void signal_handler();
int num_is_found(int* sh_m, pid_t child, unsigned arr[NUM_OF_ITEMS]);
void do_Son(int i, sem_t* mutex[], int* sh_m);
void check_fork(pid_t status);
void delete_shm(int shm_id);
void do_process(sem_t* mutex[], int* sh_m);
void win_child(unsigned array[NUM_OF_ITEMS], sem_t* mutex[], int* sh_m, int son);
void do_father(int *sh_m);
int check_flags(int flags[NUM_OF_ITEMS]);

int end = 0;
//------------- Main section -------------------
int main() {
	signal(SIGUSR1, signal_handler);
	srand(17);
	int shm_id, *shm = check_fun(&shm_id);
	sem_t* mutex[2];

	open_mutex(mutex);
	do_process(mutex, shm);
	wait(NULL);
	close_mutex(mutex);
	delete_shm(shm_id);

	return EXIT_SUCCESS;
}
//--------------------------------------------------
//--------------------------------------------------
void check_fork(pid_t status) {
	if (status < 0) {
		perror("cannot fork()");
		exit(EXIT_FAILURE);
	}
}

//--------------------------------------------------
void signal_handler() {
	end = 1;

}
;
//--------------------------------------------------
int * check_fun(int *shm_ptr) {
	int *shm_id;
	key_t key;
	if ((key = ftok("/.", 'w')) == -1) {
		perror("ftok() failed");
		exit(EXIT_FAILURE);
	}

	*shm_ptr = shmget(key, SHM_SIZE * sizeof(int), IPC_CREAT | IPC_EXCL | 0600);
	if (*shm_ptr == -1) {
		perror("shgmet failed");
		exit(EXIT_FAILURE);
	}

	shm_id = (int *) shmat(*shm_ptr, NULL, 0);
	if (shm_id == (int *) -1) {
		perror("shmat failed");
		exit(EXIT_FAILURE);
	}
	return shm_id;

}

//--------------------------------------------------
void open_mutex(sem_t * mutex[]) {
	int i;
	for (i = 0; i < 2; i++) {
		mutex[i] = sem_open("/my_mutex15", O_CREAT, 0600, 1);
		if (mutex[i] == SEM_FAILED) {
			perror("parent sem_open failed (my_mutex)");
			exit(EXIT_FAILURE);
		}
	}
}

//--------------------------------------------------
void close_mutex(sem_t *mutex[]) {
	int i;
	for (i = 0; i < 2; i++) {
		sem_close(mutex[i]);
		sem_unlink("my_mutex15");
	}
}

//--------------------------------------------------
int num_is_found(int* sh_m, pid_t child, unsigned arr[NUM_OF_ITEMS]) {
	int i, num1, num2;
	signal(SIGUSR1, signal_handler);
	num1 = sh_m[2];
	num2 = sh_m[3];
	sh_m[2]=0;
	sh_m[3]=0;

	if (sh_m[0] == child) {
		for (i = 0; i < NUM_OF_ITEMS; i++) {
			if (arr[i] == num1) {
				return i;
			}
		}
	} else if (sh_m[1] == child) {
		for (i = 0; i < NUM_OF_ITEMS; i++) {
			if (arr[i] == num2) {
				return i;
			}
		}
	}

	return -1;
}
//--------------------------------------------------
void do_Son(int i, sem_t* mutex[], int* sh_m) {
	int num1, j;
	unsigned arr[NUM_OF_ITEMS];
	int flag[NUM_OF_ITEMS] = { 0 };

	sem_wait(mutex[i]);
	sem_post(mutex[i]);

	sh_m[i] = getpid();
	for (j = 0; j < NUM_OF_ITEMS; j++)
		arr[j] = (rand() % 100);
	j = 0;
	while (1) {
		num1 = num_is_found(sh_m, getpid(), arr);
		if (num1 != -1) {
			flag[num1] = -1;
		}
		if (check_flags(flag) == 1) {
			kill(getppid(), SIGUSR1);
			win_child(arr, mutex, sh_m, i);

			break;
		}
	}
}
//--------------------------------------------------
void Print(unsigned arr[NUM_OF_ITEMS]) {
	int m;
	for (m = 0; m < NUM_OF_ITEMS; m++)
		printf("%d ", arr[m]);
	puts("\n");

}
//--------------------------------------------------
void delete_shm(int shm_id) {
	if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
		perror("shmctl failed");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------
void do_process(sem_t* mutex[], int* sh_m) {
	int j = 0;

	pid_t child_a, child_b;

	child_a = fork();
	check_fork(child_a);

	if (child_a == 0) {
		do_Son(j, mutex, sh_m);
		exit(EXIT_SUCCESS);
	} else {
		child_b = fork();
		check_fork(child_b);
		if (child_b == 0) {
			j++;
			do_Son(j, mutex, sh_m);
		} else {
			do_father(sh_m);
		}
	}
}
//--------------------------------------------------
void win_child(unsigned array[NUM_OF_ITEMS], sem_t* mutex[], int* sh_m, int son) {

	if (son == 0) {
		kill(sh_m[1], SIGKILL);
		sem_post(mutex[1]);
		printf("Child %d Covered, Child %d not Covered \n", son, 1);
		Print(array);

	} else if (son == 1) {
		kill(sh_m[0], SIGKILL);
		sem_post(mutex[0]);
		printf("Child %d Covered, Child %d not Covered \n", son, 0);
		Print(array);
	}
}
//--------------------------------------------------
void do_father(int *sh_m) {
	unsigned rand_num;
	while (1) {

		rand_num = (rand() % 100);
		if (sh_m[2] == 0) {
			sh_m[3] = rand_num;
		} else if (sh_m[3] == 0) {
			sh_m[2] = rand_num;
		}
		if (end)
			break;

	}
}
//--------------------------------------------------
int check_flags(int flags[NUM_OF_ITEMS]) {
	int i;
	for (i = 0; i < NUM_OF_ITEMS; i++) {
		if (flags[i] != -1)
			return 0;
	}
	return 1;
}
//--------------------------------------------------

