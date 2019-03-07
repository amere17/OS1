/* Ex #9a: find the first child thats done to fill his array with prime numbers
 * and then print the array 
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
 * Input: Nothing
 * ----------------------------------------------------------------------------
 * Output: the first child done to fill his array with prime numbers 
 * & print the array
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

//------------- Function section -------------------
int * check_fun(int *shm_ptr);
void Print(int arr[NUM_OF_ITEMS]);
void open_mutex(sem_t* mutexes[]);
void close_mutex(sem_t* mutexes[]);
void signal_handler();
int is_prime(int num);
void do_Son(int i, sem_t* mutex[], int* sh_m);
void check_fork(pid_t status);
void delete_shm(int shm_id);
void do_process(sem_t* mutex[], int* sh_m);
void win_child(int array[NUM_OF_ITEMS], sem_t* mutex[], int* sh_m, int son);

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

}
;

//--------------------------------------------------
int * check_fun(int *shm_ptr) {
	int *shm_id;
	key_t key;
	if ((key = ftok("/.", 'p')) == -1) {
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
		mutex[i] = sem_open("/my_mutex1", O_CREAT, 0600, 1);
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
		sem_unlink("my_mutex1");
	}
}

//--------------------------------------------------
int is_prime(int num) {
	int i;
	if (num <= 1)
		return 0;
	if (num % 2 == 0 && num > 2)
		return 0;
	for (i = 3; i < num / 2; i += 2) {
		if (num % i == 0)
			return 0;
	}
	return 1;
}
//--------------------------------------------------
void do_Son(int i, sem_t* mutex[], int* sh_m) {

	int num1, j = 0, arr[NUM_OF_ITEMS];

	sem_wait(mutex[i]);
	sem_post(mutex[i]);

	sh_m[i] = getpid();

	while (1) {
		num1 = (rand() % 998) + 2;
		if (is_prime(num1)) {
			arr[j] = num1;
			if (j == NUM_OF_ITEMS - 1) {
				win_child(arr, mutex, sh_m, i);

			}
			j++;
			sleep(rand() % 2);
		}
	}
}
//--------------------------------------------------
void Print(int arr[NUM_OF_ITEMS]) {
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
	int j;
	pid_t child;
	for (j = 0; j < NUM_OF_SONS; j++) {
		child = fork();
		check_fork(child);
		if (child == 0) {
			do_Son(j, mutex, sh_m);
			exit(EXIT_SUCCESS);
		}
	}

}
//--------------------------------------------------
void win_child(int array[NUM_OF_ITEMS], sem_t* mutex[], int* sh_m, int son) {

	if (son == 0) {
		kill(sh_m[1], SIGKILL);
		sem_post(mutex[1]);
		printf("Child %d Win, Child %d Loser \n", son, 1);
		Print(array);

	} else if (son == 1) {
		kill(sh_m[0], SIGKILL);
		sem_post(mutex[0]);
		printf("Child %d Win, Child %d Loser \n", son, 0);
		Print(array);
	}
	kill(getppid(), SIGUSR1);
}
//--------------------------------------------------
