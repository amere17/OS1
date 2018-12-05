/* Ex #3b: find the max and min , move information through pipe between father
 * and 5 sons then sort the arrays with the sons and move it again to
 * the father  at the end the father merge the new arrays.
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
 * Output: 1
 *	       986
 */

//------------------------ Include Section ------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <wait.h>
#include <math.h>
//------------------------- Define Section ------------------------------------
#define ARR_SIZE 500
#define MAX_RAND_NUM 1000
#define NUM_OF_CHILDRENS 5
#define ARR_SIZE_CHILD 100

//-------------------------- Prototype Section --------------------------------
void swap(int* a, int* b);
void quickSort(int arr[], int low, int high);
int partition(int arr[], int low, int high);
void printFunc(int arr[ARR_SIZE]);
void doSon(int num_of_son, int array[], int pipe_descs[]);
void doFather(int pipe_descs[], int index, int arr[ARR_SIZE]);
void checkPipe(int pipe_descs[]);
void checkFork(pid_t status);
//---------------------------- Main -------------------------------------------
int main() {
	pid_t status;
	int index;
	int numbers_arr[ARR_SIZE], arr[ARR_SIZE];
	int pipe_descs[2];
	srand(17);

	for (index = 0; index < ARR_SIZE; ++index) {

		numbers_arr[index] = rand() % MAX_RAND_NUM;
	}

	for (index = 0; index < NUM_OF_CHILDRENS; index++) {
		checkPipe(pipe_descs);
		status = fork();

		checkFork(status);

		if (status == 0){
			doSon(index, numbers_arr, pipe_descs);
			exit(EXIT_SUCCESS);
		}

		waitpid(status,NULL,0);
		doFather(pipe_descs, index, arr);

	}
	quickSort(arr, 0, ARR_SIZE);
	printFunc(arr);

	return EXIT_SUCCESS;
}
//------------------------- Do son Function -----------------------------------
void doSon(int num_of_son, int array[], int pipe_descs[]) {
	int i, arr[ARR_SIZE_CHILD] = { }, ret, j = 0;
close(pipe_descs[0]);
	for (i = num_of_son * 100; i < num_of_son * 100 + 100; i++) {
		arr[j] = array[i];
		j++;
	}

	quickSort(arr, 0, ARR_SIZE_CHILD);

	ret = write(pipe_descs[1], arr, sizeof(int) * ARR_SIZE_CHILD + 1);
	if (ret < 0) {
		printf("write failed\n");
		exit(1);
	}

	close(pipe_descs[1]);
	exit(0);

}
//-------------------------- Print Function -----------------------------------
void printFunc(int arr[ARR_SIZE]) {
	printf("%d", arr[0]);
	putchar('\n');
	printf("%d", arr[ARR_SIZE - 1]);
	putchar('\n');
}
//-----------------------------------------------------------------------------
void doFather(int pipe_descs[], int index, int arr[ARR_SIZE]) {
	int i, buff[ARR_SIZE_CHILD] = { }, ret, j = 0;
	close(pipe_descs[1]);
	ret = read(pipe_descs[0], buff, sizeof(int) * 100);
	if (ret < 0) {
		printf("read failed\n");
		exit(1);
	}

	for (i = index * 100; i != index * 100 + 100; i++) {
		arr[i] = buff[j];
		j++;
	}
	close(pipe_descs[0]);

}
//----------------------------- check fork function ---------------------------
void checkFork(pid_t status) {
	if (status == -1) {
		perror("Cannot fork\n");
		exit(EXIT_FAILURE);
	}
}
//----------------------------- Check Pipe ------------------------------------
void checkPipe(int pipe_descs[]) {
	if (pipe(pipe_descs) == -1) {
		perror("Cannot open pipe");
		exit(EXIT_FAILURE);
	}
}
//-----------------------------------------------------------------------------
//------------------------- Quicksort Functions -------------------------------
void quickSort(int arr[], int low, int high) {
	if (low < high) {
		/* pi is partitioning index, arr[p] is now
		 at right place */
		int pi = partition(arr, low, high);

		// Separately sort elements before
		// partition and after partition
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}
//-----------------------------------------------------------------------------
int partition(int arr[], int low, int high) {
	int pivot = arr[high];    // pivot
	int i = (low - 1), j;  // Index of smaller element

	for (j = low; j <= high - 1; j++) {
		// If current element is smaller than or
		// equal to pivot
		if (arr[j] <= pivot) {
			i++;    // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}
//-----------------------------------------------------------------------------
void swap(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}
//-----------------------------------------------------------------------------
