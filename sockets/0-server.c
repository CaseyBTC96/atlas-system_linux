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
