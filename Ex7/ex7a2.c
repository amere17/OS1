/* Ex #7a2: client to run 3 methods from the server  
 * (using rpc )
 * =============================================================================
 * Written by
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
#include "/usr/include/rpc/rpc.h"
#include "ex7_rpcgen.h"

// ============================== Defines ================================== //
#define BUFLEN    100

// ================================ Main =================================== //
int main(int argc, char** argv) {

	CLIENT *cl;

	char result[BUFLEN],*server,str[BUFLEN] = "",*str_cpy;
    double x;
	int msg_type;

	// keep the answer from the server
	int* num_result;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <host ip> \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	server = argv[1];     // Save value of server ip

	cl = clnt_create(server, RPC_PROG, CURR_VER, "tcp");
	if (cl == NULL) {    // Couldn't establish connection with server
		clnt_pcreateerror(server);
		exit(EXIT_FAILURE);
	}

	do {

		scanf("%d", &msg_type);

		if (msg_type == 1) {
			scanf("%s",str);
            str_cpy = &str[0];
            num_result = smallletter_1(&str_cpy, cl);
	        sprintf(result, "%d", *num_result);

		} else if (msg_type == 2) {
	        scanf("%lf", &x);
            num_result = sumofrational_1(&x, cl);
	        sprintf(result, "%d", *num_result);

		} else if (msg_type == 3) {
            scanf("%s",str);
            str_cpy = &str[0];
            num_result = sameletter_1(&str_cpy, cl);
	        sprintf(result, "%d", *num_result);

		} else {
			clnt_destroy(cl);
			exit(EXIT_SUCCESS);
		}
		if (result == NULL)
		{

			clnt_perror(cl, server);
			exit(EXIT_FAILURE);
		}

		printf("Result returned from server is: %s\n", result);

	} while (1);

	return (EXIT_SUCCESS);
}
