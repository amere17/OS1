#include<stdio.h>
#include<stdlib.h>
#include <time.h>

void towerOfHanoi(int n, char from_rod, char to_rod, char aux_rod, FILE *file); //hanoi algorithim

//main
int main(int argc, char **argv) {

	//no string was passed
	if (argc != 2)
	{
		puts("Error: Usage ./child int");
		exit(EXIT_FAILURE);
	}

	int num_of_disks = atoi(argv[1]);
	FILE *file = (fopen("nul", "w"));
	towerOfHanoi(num_of_disks, 'a', 'c', 'b', file);
	return EXIT_SUCCESS;

}


//Hanoi algorithim
void towerOfHanoi(int n, char from_rod, char to_rod, char aux_rod, FILE *file)
{
	if (n == 1)
	{
		fprintf(file, "Move disk 1 from %c to %c\n", from_rod, to_rod);
		return;
	}
	towerOfHanoi(n - 1, from_rod, aux_rod, to_rod, file);
	fprintf(file, "Move disk %d from %c to %c\n", n, from_rod, to_rod);
	towerOfHanoi(n - 1, aux_rod, to_rod, from_rod, file);
}