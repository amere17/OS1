/* Ex #8a: 5 threads with 5 sorted arrays then merage all the arrays in one
 * sorted array
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
 * Output: sorted array that meraged from 5 threads.
 *
 */
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// ============================== Defines ================================== //
#define SIZE 10
#define RES_SIZE 50
#define THREADS 5

// ============================= Prototypes ================================ //
void* my_func(void *);
void my_sort();
void init_key();

//=============================== Globals====================================//
pthread_once_t threads_init = PTHREAD_ONCE_INIT;
pthread_key_t key;

// ================================ Main =================================== //
int main() {
	int round, arr[SIZE * 10] = { 0 }, status, i, *res, j, index, k = 0;
	pthread_t thread_data[THREADS];
	srand(17);

	for (i = 0; i < THREADS; i++) {
		status = pthread_create(&(thread_data[i]), NULL, my_func, (void *) arr);
		if (status != 0) {
			fputs("pthread_create failed in main", stderr);
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < THREADS; i++) {

		pthread_join(thread_data[i], (void **) &res);
		for (j = 10 * k, index = 0; j < k * 10 + 10; j++, index++) {
			arr[j] = res[index];
		}
		k++;

	}

	for (round = 0; round < RES_SIZE - 1; round++)
		for (i = 0; i < RES_SIZE - round - 1; i++)
			if (arr[i] > arr[i + 1]) {
				int temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
			}

	for (j = 0; j < SIZE * THREADS; j++)
		printf("%d ", arr[j]);
	puts("\n");

	free(res);
	return EXIT_SUCCESS;
}

// ============================= Functions ================================= //
void* my_func(void * args) {
	int rc;
	int *arr = (int *) malloc(SIZE * sizeof(int));
	int i;
	pthread_once(&threads_init, init_key);
	if (arr == NULL) {
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < SIZE; i++)
		arr[i] = rand() % 1000;

	rc = pthread_setspecific(key, arr);
	if (rc) {
		fputs("pthread_setspecific failed", stderr);
		exit(EXIT_FAILURE);
	}

	my_sort();

	pthread_exit(arr);
}

//---------------------------------------------------------------------------//
void my_sort() {
	int *arr = (int *) pthread_getspecific(key);
	int round, i;

	for (round = 0; round < SIZE - 1; round++)
		for (i = 0; i < SIZE - round - 1; i++)
			if (arr[i] > arr[i + 1]) {
				int temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
			}
}
//---------------------------------------------------------------------------//
void init_key() {
	int rc;

	puts("In init()");
	rc = pthread_key_create(&key, NULL);
	if (rc) {
		fputs("pthread_key_create failed", stderr);
		exit(EXIT_FAILURE);
	}
}
