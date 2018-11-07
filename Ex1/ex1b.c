/* Ex #1b: This program works as calculator with max and sum functions.
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
 * Input: One of the actions or exit to close the program.
 * ----------------------------------------------------------------------------
 * Output: The output is according to the action we asked for in the input.
 ******************************************************************************/

//--------------------------- include section ---------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
//-------------------------- define section -----------------------------------
#define MAX_STR_LEN 25

//------------------------ prototypes section----------------------------------
void doChild(char action[]);
void doAction(char action[],char **res);
char **str2args(char str[MAX_STR_LEN]);

//-------------------------- main ---------------------------------------------
int main() {
	pid_t status;
	char action[MAX_STR_LEN];
	fgets(action, MAX_STR_LEN, stdin);
	while ((strcmp(action, "exit\n")) != 0) {
		status = fork();
	if (status == 0)
			doChild(action);
		wait(NULL);
		puts(""); //new line
		fgets(action, MAX_STR_LEN, stdin);
	}
	return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
//----------- check what action to do and run the program of the action -------
void doChild(char action[]) {
	char **res = NULL;
	char str[MAX_STR_LEN] = { '\0' };
	res=str2args(str);
	//---------------------------------
	if (strcmp(action, "plus\n") == 0)
		doAction("plus",res);
	else if (strcmp(action, "minus\n") == 0)
		doAction("minus",res);
	else if (strcmp(action, "sum\n") == 0)
		doAction("./sum",res);
	else if (strcmp(action, "max\n") == 0)
		doAction("max",res);
	else {
		puts("error");
		exit(0);
	}
	free(res);
}
//-----------------------------------------------------------------------------
//-------------------- run action program -------------------------------------
void doAction(char action[],char **res)
{
	if (execvp(action, res) != 0) {
				perror("execvp() failed");
				exit(EXIT_FAILURE);
			}
}
//-----------------------------------------------------------------------------
//------------- arrange strings to args array and returns **res --------------- 
char **str2args(char str[MAX_STR_LEN])
{
	char **res = NULL;
	fgets(str, MAX_STR_LEN, stdin);
		str[strlen(str) - 1] = '\0';

		char *p = strtok(str, " ");
		int n_spaces = 0;

		while (p) {
			res = realloc(res, sizeof(char*) * ++n_spaces);

			if (res == NULL)
				exit(-1);
			res[n_spaces - 1] = p;
			p = strtok(NULL, " ");
		}
		res = realloc(res, sizeof(char*) * (n_spaces + 1));
		res[n_spaces] = 0;
		return res;
}
