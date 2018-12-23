/* Ex #5a: the client of two servers that find gcd or factor primes 
 * (using shared memory)
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
 * Input: choose server by g/d and put two numbers for gcd or number
 * for Primes Factor
 * ----------------------------------------------------------------------------
 * Output: calculate gcd of two numbers of find Primes Factor
 * Example : g 12 16
 * Output : 4
 *
 */
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/wait.h>

// ============================== Defines ================================== //
#define CHILDS_ID 12
#define GCD 8
#define PRIME_FACTOR 100
// ============================= Prototypes ================================ //
key_t open_shmr(char type);
void close_shm(int shmid);
void do_Process();
void do_gcd();
void do_primes();
void check_shmat(int *shm_ptr);
void get_sig();
int get_shm_id(key_t key, int size);
void catch_sigint(int sigint);
void catch_sigusr1(int sigusr1);
void catch_sigusr2(int sigusr2);
int gcd(int a, int b);
int connect_shmr(int size, char type);
void check_shmat(int *pointer);
void check_fork(pid_t status);
void prime(int *prime_shm_pointer);

int finish = 1;
// ================================ Main =================================== //
int main() {

	//------------- variables ---------------
	pid_t status;
	int index, shm_id, shm_id_gcd, shm_id_prime;
	//-------- open shared memory for father -----
	shm_id = get_shm_id(open_shmr('f'), CHILDS_ID);
	int *temp = (int*) shmat(shm_id, NULL, 0);
    check_shmat(temp);
	//---------- do three children with two shared memories ---------
    for (index = 0; index < 3; index++) {
		status = fork();
		check_fork(status);
		if (status == 0) {
			temp = (int *) shmat(connect_shmr(CHILDS_ID, 'f'), NULL, 0);
			temp[index] = getpid();
			if (index == 1) {
				shm_id_gcd = get_shm_id(open_shmr('g'), GCD);
				temp = (int*) shmat(shm_id_gcd, NULL, 0);
				do_gcd();

				close_shm(shm_id_gcd);
				exit(EXIT_SUCCESS);

			}
			if (index == 2) {
				shm_id_prime = get_shm_id(open_shmr('d'), PRIME_FACTOR);
				temp = (int*) shmat(shm_id_prime, NULL, 0);
				do_primes();

				close_shm(shm_id_prime);
				exit(EXIT_SUCCESS);
			}
			if (index == 0) {
				sleep(1);
				do_Process();
				exit(EXIT_SUCCESS);
			}
		}
	}
	waitpid(status, NULL, 0);
	close_shm(shm_id);
	exit(EXIT_SUCCESS);
}
//--------------------------- check fork ------------------------------------//
void check_fork(pid_t status) {

	if (status < 0) {
		perror("cannot fork");
		exit(EXIT_FAILURE);
	}
}
//------------------------ start shared memory ------------------------------//
key_t open_shmr(char type) {
	key_t key;
	if (((key) = ftok(".", type)) == -1) {
		perror("ftok failed");
		exit(EXIT_FAILURE);
	}
	return key;
}
//---------------------- get shared memory id -------------------------------//
int get_shm_id(key_t key, int size) {
	int shm_id;

	if (((shm_id) = shmget(key, size, IPC_CREAT | 0666)) == -1) {
		perror("shmget failed ");
		exit(EXIT_FAILURE);
	}
	return shm_id;
}
//-------------------- close shared memory ----------------------------------//
void close_shm(int shmid) {
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl failed");
		exit(EXIT_FAILURE);
	}
}
//----------------------check shared memory attach---------------------------//
void check_shmat(int *shm_ptr) {
	if (!shm_ptr) {
		perror("shmat failed");
		exit(EXIT_FAILURE);
	}
}
//---------------------- connect shared memory ------------------------------//
int connect_shmr(int size, char type) {

	key_t key;
	int shmid;
	if (((key) = ftok(".", type)) == -1) {
		perror("ftok failed");
		exit(EXIT_FAILURE);
	}
	if (((shmid) = shmget(key, size, 0666)) == -1) {
		perror("shmget failed");
		exit(EXIT_FAILURE);
	}
	return shmid;
}
//---------------------- do child of gcd ------------------------------------//
void do_gcd() {

	int *gcd_shm_pointer;
	pid_t *pointer_pid;
	pointer_pid = (int*) shmat(connect_shmr(CHILDS_ID, 'f'), NULL, 0);
	gcd_shm_pointer = (int*) shmat(connect_shmr(GCD, 'g'), NULL, 0);
	while (finish) {
		get_sig();
		pause();
		gcd_shm_pointer[0] = gcd(gcd_shm_pointer[0], gcd_shm_pointer[1]);
		kill(pointer_pid[0], SIGUSR2);
	}

	exit(EXIT_SUCCESS);
}
//---------------------- Do child  of primes --------------------------------//
void do_primes() {

	pid_t *pid_ptr;
	int *prime_ptr;
	pid_ptr = (int*) shmat(connect_shmr(CHILDS_ID, 'f'), NULL, 0);
	prime_ptr = (int*) shmat(connect_shmr(PRIME_FACTOR, 'd'), NULL, 0);
	while (finish) {
		get_sig();
		pause();
		prime(prime_ptr);
		kill(pid_ptr[0], SIGUSR2);
	}

	exit(EXIT_SUCCESS);
}
//------------------------ do front end process -----------------------------//
void do_Process() {

	int index;
	pid_t *pid_ptr;
	char choose;
	int *prime_ptr, *gcd_ptr;
	pid_ptr = (pid_t*) shmat(connect_shmr(CHILDS_ID, 'f'), NULL, 0);
	prime_ptr = (int*) shmat(connect_shmr(PRIME_FACTOR, 'd'), NULL, 0);
	gcd_ptr = (int*) shmat(connect_shmr(GCD, 'g'), NULL, 0);

	while (finish) {
		get_sig();

		scanf("%c", &choose);

		if (choose == 'g') {
			scanf("%d%d", &gcd_ptr[0], &gcd_ptr[1]);
			kill(pid_ptr[1], SIGUSR1);
			pause();
			printf("%d\n", gcd_ptr[0]);
		} else if (choose == 'd') {
			scanf("%d", &prime_ptr[0]);
			kill(pid_ptr[2], SIGUSR1);
			pause();
			for (index = 0; prime_ptr[index] != -1; index++) {
				printf("%d ", prime_ptr[index]);
			}

		}

	}

	kill(pid_ptr[1], SIGUSR1);
	kill(pid_ptr[2], SIGUSR1);
	exit(EXIT_SUCCESS);

}
//---------------------------------------------------------------------------//
void get_sig() {

	signal(SIGUSR1, catch_sigusr1);
	signal(SIGUSR2, catch_sigusr2);
	signal(SIGINT, catch_sigint);

}
//---------------------------------------------------------------------------//
void catch_sigint(int sigint) {

	get_sig();
	sigint = SIGINT;
	finish = 0;
}
//---------------------------------------------------------------------------//
void catch_sigusr1(int sigusr1) {

	get_sig();
	sigusr1 = SIGUSR1;
}
//---------------------------------------------------------------------------//
void catch_sigusr2(int sigusr2) {

	get_sig();
	sigusr2 = SIGUSR2;
}
//------------------------ find gcd of two numbers --------------------------//
int gcd(int a, int b) {
	if (a == 0)
		return b;
	return gcd(b % a, a);
}
//--------------------------find primes factor for a number -----------------//
void prime(int *prime_shm_pointer) {
	//get access to pointer

	int dev = 2;
	int index = 0;
	int number = prime_shm_pointer[0];

	while (number != 0) {
		if (number % dev != 0)
			dev++;
		else {
			number = number / dev;
			prime_shm_pointer[index] = dev;
			index++;
			if (number == 1)
				break;
		}
	}
	//get the correct length data
	prime_shm_pointer[index] = -1;
}
//---------------------------------------------------------------------------//
