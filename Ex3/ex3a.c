/*
 *  DESCRIBED IN README
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
//-----------------------------------------------------------------------------
#define SIZE 5
#define TOWER1 10
#define TOWER2 15
#define TOWER3 20
#define TOWER4 25
#define TOWER5 30
//-----------------------------------------------------------------------------
void do_father	(int num_of_disks, int arr[SIZE], int pipe_descs[2]);
void do_child	(int num_of_disks);
void solve_tower(int num_of_disks, int pipe_descs[2]);
void tower_of_hanoi(int n, char from_rod, char to_rod, char aux_rod);
void check_fork(pid_t status);
void check_pipe(int pipe_descs[2]);
//-----------------------------------------------------------------------------
int main() {
	pid_t status;						// keep the current process ID

	int i, tower[SIZE] = { TOWER1, TOWER2, TOWER3, TOWER4, TOWER5 };

	for (i = 0; i < SIZE; i++) {
		status = fork();
		check_fork(status);
		if (status == 0) {
			do_child(tower[i]);
			exit(EXIT_SUCCESS);
		}
		waitpid(status, NULL, 0); // wait for the last child process
	}

	return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
// son: generates procs to solve towers
void do_child(int num_of_disks) {
	pid_t status;
	int i;
	int arr[SIZE];
	int pipe_descs[2];

	for (i = 0; i < SIZE; i++) {
		check_pipe(pipe_descs); 		// new pipe
		status = fork();				// new child
		check_fork(status);				// check child
		if (status == 0) {
			solve_tower(num_of_disks, pipe_descs);
			exit(EXIT_SUCCESS);
		}
	}	wait(NULL);
		do_father(num_of_disks, arr, pipe_descs);

	exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// father: calculates the min, max & average
void do_father(int num_of_disks, int arr[SIZE], int pipe_descs[2]) {
	int i, time, sum=0, min=50000, max=0;
	close(pipe_descs[1]);
	close(STDIN_FILENO);
	dup(pipe_descs[0]);
	for(i=0;i<SIZE;i++){
		scanf("%d", &time);
		sum += time;
		if(time > max)
			max = time;
		if(time < min)
			min = time;
	}
	printf("Num of disks = %d. min %d, max %d, avrg %f\n",
			num_of_disks, min, max, (float)(sum/SIZE));
	close(pipe_descs[0]);

}

//-----------------------------------------------------------------------------
//------------------------------ returns time of one process ------------------
void solve_tower(int num_of_disks, int pipe_descs[2]) {
	time_t second, temp_time;
	second = time(NULL);
	tower_of_hanoi(num_of_disks, 'a', 'b', 'c');
	temp_time = time(NULL);
	second = temp_time - second;


	close(pipe_descs[0]);
	close(STDOUT_FILENO);
	dup(pipe_descs[1]);
	printf("%d", (int) second);
	close(pipe_descs[1]);

}
//-----------------------------------------------------------------------------
//---------------------------- hanoi function using recursion -----------------
void tower_of_hanoi(int n, char from_rod, char to_rod, char aux_rod) {
	if (n == 1)
		return;
	tower_of_hanoi(n - 1, from_rod, aux_rod, to_rod);
	tower_of_hanoi(n - 1, aux_rod, to_rod, from_rod);
}

//-----------------------------------------------------------------------------
// check if fork()
void check_fork(pid_t status) {
	if (status == -1) {
		perror("cannot fork()");
		exit(EXIT_FAILURE);
	}
}
//-----------------------------------------------------------------------------
// check if pipe()
void check_pipe(int pipe_descs[2]) {
	if (pipe(pipe_descs) == -1) {
		perror("cannot pipe()");
		exit(EXIT_FAILURE);
	}
}
//-----------------------------------------------------------------------------
