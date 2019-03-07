/* Ex #8b:find the array thats covered from the random numbers 
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
 * Output: number of thread thats his array covered .
 *
 */
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>

// ============================== Defines ================================== //
#define SIZE 4
#define N 20
#define RANGE 1000

// ============================= Prototypes ================================ //
void *my_func(void *);
void kill_threads(void);
void init_key(void);
void cleanup_malloc(void *arg);

// ============================== Globals ================================== //
pthread_once_t threads_init = PTHREAD_ONCE_INIT;
pthread_once_t threads_kill = PTHREAD_ONCE_INIT;
pthread_key_t key;
pthread_t thread_data[SIZE];
// ================================ Main =================================== //
int main() {
	int status, i;
	int matrix[SIZE][N];

	int j, fillNum;

	for (i = 0; i < SIZE; ++i) {
		matrix[i][0] = i;
		for (j = 1; j < N + 1; ++j) {

			fillNum = rand() % RANGE + 1; // limit up to MAX_RANGE
			matrix[i][j] = fillNum;

		}
	}

	for (i = 0; i < SIZE; i++) {
		status = pthread_create(&(thread_data[i]), NULL, my_func, matrix[i]);

		if (status != 0) {
			fputs("thread create failed in  main", stderr);
			exit(EXIT_FAILURE);
		}
	}

	 // main thread wait for all others threads
	    for (i = 0; i < SIZE; i++)
	        pthread_join(thread_data[i], NULL);
	return EXIT_SUCCESS;
}

// ============================= Functions ================================= //
void *my_func(void * args) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	pthread_once(&threads_init, init_key);
	int num, j, done = 0;
	int *arr = (int*) args;
	int *arr1 = (int *) malloc(sizeof(int));
	srand(17);

	pthread_cleanup_push(cleanup_malloc, (void *) arr1);

		while (1) {
			num = rand() % RANGE + 1;
			for (j = 1; j < N + 1; j++) {
				if (arr[j] == num)
					arr1[j] = 0;
			}
			for (j = 1; j < N + 1; j++) {
				if (arr1[j] == 0)
					done = 1;
				else {
					done = 0;
					break;
				}

			}
			arr1=arr;
			if (done == 1) {
				if (pthread_setspecific(key, arr1)) {
					fputs("pthread_setspecific failed", stderr);
					exit(EXIT_FAILURE);
				}
				pthread_once(&threads_kill, kill_threads);
				pthread_exit(NULL);

			}
		}
		pthread_cleanup_pop(0);

}
//---------------------------------------------------------------------------//
void kill_threads() {
	int i;
	pthread_t curr;

	curr = pthread_self();
	for (i = 0; i < SIZE; i++) {
		if (thread_data[i] != curr) {
			pthread_cancel(thread_data[i]);
		}
	}
	int *threadF = (int *) pthread_getspecific(key);

	printf("%d\n", *threadF);

}

//------------------------------------------------------------------------------
void init_key() {
	if (pthread_key_create(&key, NULL)) {
		fputs("pthread_key_create failed", stderr);
		exit(EXIT_FAILURE);
	}
}

//------------------------------------------------------------------------------
//Cleanup method of malloc
void cleanup_malloc(void *arg) {
	free((char *) arg);
}

