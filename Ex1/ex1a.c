/* Ex #1a: This program solving hanoi tower problem in recursion with father
* process and child 
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
* Input: NULL file.
* -----------------------------------------------------------------------------
* output: each tower = time of the process.
*
******************************************************************************/

//include section
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

//define(s) section.
#define size 5
#define tower1 10
#define tower2 20
#define tower3 25
#define tower4 30
#define tower5 35

//-------------------------- prototypes section -------------------------------
void tower_of_hanoi(FILE *f,int n,char from_rod,  char to_rod,  char aux_rod);
void check_fork(pid_t status);
void solve_tower(FILE *f,int num_of_disks);
void father_process(pid_t status);

//------------------------------- main ----------------------------------------
int main(){
	int j,child[size] = {tower1,tower2,tower3,tower4,tower5};
	pid_t  status;

	FILE *fptr;
	    fptr = fopen("/dev/null", "w");


	for(j=0; j<size; j++){

		status = fork();
		check_fork(status);
		if(status == 0){
			solve_tower(fptr,child[j]);
			exit(EXIT_SUCCESS);
		}
		father_process(status);
	}
	fclose(fptr);
	exit(EXIT_SUCCESS);
}
//-----------------------------------------------------------------------------
//--------------------------------- do child process --------------------------
void father_process(pid_t status)
{
	waitpid(status, NULL,0);
}
//-----------------------------------------------------------------------------
//---------------------------- check the new process --------------------------
void check_fork(pid_t status)
{
	if(status < 0){
				perror("cannot fork()");
				exit(EXIT_FAILURE);
			}
}
//-----------------------------------------------------------------------------
//------------------------------ returns time of one process ------------------
void solve_tower(FILE *f,int num_of_disks)
{	time_t second, temp_time;
	second = time (NULL);
				tower_of_hanoi(f,num_of_disks,'a','b','c');
				temp_time =time(NULL);
				second = temp_time - second;
				printf("%d = %d \n",num_of_disks, (int)second);
}
//-----------------------------------------------------------------------------
//---------------------------- hanoi function using recursion -----------------
void tower_of_hanoi(FILE *f,int n,char from_rod,  char to_rod,  char aux_rod){

	if (n == 1)
	    {
	        fprintf(f," Move disk 1 from rod %c to rod %c\n", from_rod, to_rod);
	        return;
	    }
		tower_of_hanoi(f,n-1, from_rod, aux_rod, to_rod);
	    fprintf(f,"Move disk %d from rod %c to rod %c\n", n, from_rod, to_rod);
	    tower_of_hanoi(f,n-1, aux_rod, to_rod, from_rod);
}
//-----------------------------------------------------------------------------
