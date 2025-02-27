#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define EXIT_S 0
#define EXIT_F 1

/**
* main - Connects to a listening server.
*
* @argc: arg count.
* @argv: arg array.
* Return: EXIT_S on success | EXIT_F on failure.
*/
int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *p;
	int sockfd, status;

	if (argc != 3) /* Check for the correct number of arguments */
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_F);
	}
	memset(&hints, 0, sizeof(hints)); /* Initialize hints */
	hints.ai_family = AF_INET; /* Use IPv4 */
	hints.ai_socktype = SOCK_STREAM; /* TCP socket */
	status = getaddrinfo(argv[1], argv[2], &hints, &res);
	if (status != 0) /* Get address info */
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return (EXIT_F);
	}
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1)
	{ /* Create a socket */
		perror("socket"), freeaddrinfo(res);
		return (EXIT_F);
	}
	for (p = res; p != NULL; p = p->ai_next)
	{ /* Attempt to connect */
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0)
		{
			printf("Connected to %s:%s\n", argv[1], argv[2]);
			break; /* Connection successful */
		}
	}
	if (p == NULL) /* Check if we were able to connect */
	{
		fprintf(stderr, "Connection failed\n");
		close(sockfd), freeaddrinfo(res);
		return (EXIT_F);
	}
	freeaddrinfo(res); /* Clean up */
	close(sockfd);
	return (EXIT_S);
}
