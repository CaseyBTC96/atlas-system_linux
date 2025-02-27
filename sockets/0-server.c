#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define PORT 12345

/**
 * main - Initializes and runs a simple TCP server.
 * This function creates a TCP socket, binds it to a specified port (12345),
 * and listens for incoming connections. The server runs indefinitely,
 * waiting for signals to handle incoming requests. Error handling is
 * implemented for socket creation, binding, and listening.
 * Return: 0 upon successful execution (though unreachable in this case).
 */
int main(void)
{
	int sockfd;
	struct sockaddr_in server_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0); /* Create a socket */
	if (sockfd < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	/* Prepare the sockaddr_in structure */
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; /* IPv4 optimizes direction used*/
	server_addr.sin_addr.s_addr = INADDR_ANY;  /* Any address */
	server_addr.sin_port = htons(PORT);  /* Port number*/
	/* Bind the socket */
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	if (listen(sockfd, 5) < 0) /* Listen on the socket */
	{
		perror("Listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);
	while (1) /* Hang indefinitely */
	{
		pause();  /* Suspend the process until a signal is received */
	}
	close(sockfd); /* Close the socket (not reachable) */
	return (0);
}
