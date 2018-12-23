/* Ex #6a3: the client of two servers that find gcd or factor primes
 * (using socket)
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
 * Input: 1 for primes factor and 0 for gcd
 * then write the input of the gcd/prime factor
 * ----------------------------------------------------------------------------
 * Output: calculate gcd of two numbers of find Primes Factor
 * Example : 0 12 16
 * Output : 4
 *
 */
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

// ============================== Defines ================================== //
#define  BUFLEN        1024
#define GCD_SERVER "3879"
#define PF "3880"
#define ARGC  5
// ============================= Prototypes ================================ //
void memset_s(struct addrinfo *con_kind);
void open_socket(int *socket_name, struct addrinfo **addr_info_res);
void connect_socket(const int *socket_name, ssize_t *rc,
		struct addrinfo **addr_info_res);
void getaddrinfo_s(ssize_t *rc, char *server_ip, char *server_port,
		struct addrinfo *con_kind, struct addrinfo **addr_info_res);
void Check_argc(int argc, char *argv[]);
void Check_socket(int *mySocket);
void Check_rc(int *rc);

// ================================ Main =================================== //
int main(int argc, char *argv[]) {

	//variables
	ssize_t rc1, rc2;
	int i, app2, app1, ans, num1, num2, prime, result_gcd, result_prime[50];
	struct addrinfo con_kind, *addr_info_res1;
	struct addrinfo con_kind2, *addr_info_res2;

	//check_argc
	Check_argc(argc, argv);

	//get memset
	memset_s(&con_kind);
	memset_s(&con_kind2);

	//get addrinfo
	getaddrinfo_s(&rc1, argv[1], argv[2], &con_kind, &addr_info_res1);
	getaddrinfo_s(&rc2, argv[3], argv[4], &con_kind2, &addr_info_res2);

	//open socket
	open_socket(&app1, &addr_info_res1);
	open_socket(&app2, &addr_info_res2);

	//connect socket
	connect_socket(&app1, &rc1, &addr_info_res1);
	connect_socket(&app2, &rc2, &addr_info_res2);

	while (1) {
		scanf("%d", &ans);

		if (ans == 2) {
			close(app2);
			freeaddrinfo(addr_info_res1);
			close(app1);
			freeaddrinfo(addr_info_res2);
			exit(EXIT_SUCCESS);
		} else if (ans == 1) {

			scanf("%d", &prime);
			write(app2, &prime, sizeof(prime));
			rc1 = read(app2, &result_prime, sizeof(result_prime));

			for (i = 0; i < sizeof(result_prime) && result_prime[i]!=-1; i++)
				printf("%d ", result_prime[i]);
			puts("\n");
		} else if (ans == 0) {
			scanf("%d%d", &num1, &num2);
			write(app1, &num1, sizeof(num1));
			write(app1, &num2, sizeof(num2));
			rc1 = read(app1, &result_gcd, sizeof(result_gcd));
			printf("%d\n", result_gcd);
		}
	}

	return EXIT_SUCCESS;
}
// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
void Check_argc(int argc, char *argv[]) {
	if (argc < ARGC) {
		fprintf(stderr, "Usage: <port server 1> <ip"
				"server 1> <port server 2> <ip server 2>  \n", argv[0]);
		exit(EXIT_FAILURE);
	}
}
//---------------------------------------------------------------------------//
void Check_rc(int *rc) {
	if (rc) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
}
//---------------------------------------------------------------------------//
void Check_socket(int *mySocket) {
	if (mySocket < 0) {
		perror("socket: allocation failed");
		exit(EXIT_FAILURE);
	}
}
//---------------------------------------------------------------------------//
void memset_s(struct addrinfo *con_kind) {
	memset((*&con_kind), 0, sizeof(*con_kind));
	(*con_kind).ai_family = AF_UNSPEC;          // set that any group suitable
	(*con_kind).ai_socktype = SOCK_STREAM;
}

//---------------------------------------------------------------------------//
void getaddrinfo_s(ssize_t *rc, char *server_ip, char *server_port,
		struct addrinfo *con_kind, struct addrinfo **addr_info_res) {
	if (((*rc) = getaddrinfo(server_ip, server_port, (*&con_kind),
			(*&addr_info_res)) != 0)) {
		fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror((int) (*rc)));
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------//
void open_socket(int *socket_name, struct addrinfo **addr_info_res) {
	(*socket_name) = socket((*addr_info_res)->ai_family,
			(*addr_info_res)->ai_socktype, (*addr_info_res)->ai_protocol);

	Check_socket(socket_name);
}

//---------------------------------------------------------------------------//
void connect_socket(const int *socket_name, ssize_t *rc,
		struct addrinfo **addr_info_res) {

	(*rc) = connect((*socket_name), (*addr_info_res)->ai_addr,
			(*addr_info_res)->ai_addrlen);

	Check_rc(*rc);
}
