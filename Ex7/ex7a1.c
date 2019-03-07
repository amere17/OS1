/* Ex #7a1: server with all the functions that we need to use in client 
 * (using rpc )
 * =============================================================================
 *Written by
 * Muhammad Egbariya - 305140931 & Mohamad Amer - 315336115
 * login
 * muhammadeg / mohamadam
 * Course/Year
 * OS1 2019
 * Year
 * B
 * =============================================================================
 * Input: Nothing.
 * ----------------------------------------------------------------------------
 * Output: Nothing.
 *
 */
// ============================== Include ================================== //
#include <stdio.h>
#include <ctype.h>
#include "/usr/include/rpc/rpc.h"
#include "ex7_rpcgen.h"
#include <math.h>
#include <string.h>

// ============================= Functions ================================= //
//---------------------------------------------------------------------------//

int * smallletter_1_svc(char** str, struct svc_req *foo) {
	static int result;
	char *c;
	sscanf(*str, "%s", c);
	int i = 0;
	for (i = 0; i < strlen(c); i++) {
		
		if (!isalpha(c[i]) || isupper(c[i])) {
			result = 0;
			return (&result);
		}

	}
	result = 1;
	return (&result);

}

//---------------------------------------------------------------------------//
int *sumofrational_1_svc(double *x, struct svc_req *foo) {
	static int result;

	char num[50];
	int i = 0, sum = 0;
       snprintf(num, 50, "%lf\n", *x);


	for (i = 0; i < 50; i++) {
		if (isdigit(num[i])) {
			sum += num[i] - 48;
		}
	}

	result = sum;
	return (&result);
}
//---------------------------------------------------------------------------//
int * sameletter_1_svc(char** msg, struct svc_req *foo) {
	static int result;

	int count = 0, i, j;
	char str1[20], str2[20];
	sscanf(*msg, "%s %s", &str1, &str2);
	
	for (i = 0; i < strlen(str1); i++) {
		for (j = 0; i < strlen(str2); j++) {
			if (str1[i] == str2[j]) {
				count++;
			}
		}
	}
	result = count;
	return (&result);
}
