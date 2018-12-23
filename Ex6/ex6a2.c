/* Ex #6a2:get number, to find factor primes of it and
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
#define PF_PORT "3880"
#define BUFLEN 50

// ================================ Main =================================== //
int main() {
	//variables
	int rc, num, i, j, isPrime, primes[50], index = 0, fd;
	int main_socket, serving_socket;
	struct addrinfo con_kind, *addr_info_res;
	fd_set rfd, c_rfd;

	// setting pointer
	memset(&con_kind, 0, sizeof con_kind);
	con_kind.ai_family = AF_UNSPEC;
	con_kind.ai_socktype = SOCK_STREAM;
	con_kind.ai_flags = AI_PASSIVE;

	// getaddrinfo
	if ((rc = getaddrinfo(NULL, PF_PORT, &con_kind, &addr_info_res)) != 0) {
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
				rc = read(serving_socket, &num, sizeof(num));
				if (rc == 0) {
					close(fd);
					FD_CLR(fd, &rfd);
				} else if (rc > 0) {
					index = 0;
					for (i = 2; i <= num; i++) {

						if (num % i == 0) {
							isPrime = 1;
							for (j = 2; j <= i / 2; j++) {
								if (i % j == 0) {
									isPrime = 0;
									break;
								}
							}
							if (isPrime == 1) {
								primes[index] = i;
								index++;
							}
						}
					}
					primes[index] = -1;
					write(serving_socket, &primes, sizeof(primes));
					}

				}
			}

			if (rc < 0) {
				perror("read failed");
				exit(EXIT_FAILURE);
			}

		}
		return EXIT_SUCCESS;
	}
