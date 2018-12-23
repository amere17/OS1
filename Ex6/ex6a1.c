/* Ex #6a1:find the GCD of the two numbers and
 * send the answer using socket
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
 * Output: Nothing.
 *
 */
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
// ============================== Defines ================================== //
#define GCD_PORT "3879"
#define BUFLEN 50

// ================================ Main =================================== //
int main() {
	int rc, num1, num2, i = 1, gcd, fd;
	int main_socket, serving_socket;
	struct addrinfo con_kind, *addr_info_res;
	fd_set rfd, c_rfd;

	// setting pointer
	memset(&con_kind, 0, sizeof con_kind);
	con_kind.ai_family = AF_UNSPEC;
	con_kind.ai_socktype = SOCK_STREAM;
	con_kind.ai_flags = AI_PASSIVE;

	// getaddrinfo
	if ((rc = getaddrinfo(NULL, GCD_PORT, &con_kind, &addr_info_res)) != 0) {
		fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(rc));
		exit(EXIT_FAILURE);
	}

	// socket
	main_socket = socket(addr_info_res->ai_family, addr_info_res->ai_socktype,
			addr_info_res->ai_protocol);
	if (main_socket < 0) {
		perror("socket: allocation failed");
		exit(EXIT_FAILURE);
	}

	// bind
	rc = bind(main_socket, addr_info_res->ai_addr, addr_info_res->ai_addrlen);
	if (rc) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// listen
	rc = listen(main_socket, 5);
	if (rc) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	FD_ZERO(&rfd);
	FD_SET(main_socket, &rfd);
	while (1) {
		c_rfd = rfd;

		// accept
		rc = select(getdtablesize(), &c_rfd, NULL, NULL, NULL);
				if (FD_ISSET(main_socket, &c_rfd)) {
					serving_socket = accept(main_socket, NULL, NULL);
					if (serving_socket >= 0) {
						FD_SET(serving_socket, &rfd);
					}
				}

		for (fd = main_socket + 1; fd < getdtablesize(); fd++) {
			if (FD_ISSET(fd, &c_rfd)) {
				rc = read(serving_socket, &num1, sizeof(num1));
				rc = read(serving_socket, &num2, sizeof(num2));
				if (rc == 0) {
					close(fd);
					FD_CLR(fd, &rfd);
				} else if (rc > 0) {
					// GCD
					for (i = 1; i <= num1 && i <= num2; ++i) {
						// Checks if i is factor of both integers
						if (num1 % i == 0 && num2 % i == 0)
							gcd = i;
					}
					write(serving_socket, &gcd, sizeof(gcd));
				} else {
					perror("read() failed");
					exit(EXIT_FAILURE);
				}
			}
		}

	}
	return EXIT_SUCCESS;
}
