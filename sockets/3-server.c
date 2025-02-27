#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

/*
* main - Initializes and runs a simple TCP server.
* This function creates a TCP socket, binds it to a specified port (12345),
* listens for incoming connections, and handles a single client request.
* Return: 0 upon successful execution.
*/
int main(void)
{
	int sockfd, newsockfd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;

	sockfd = socket(AF_INET, SOCK_STREAM, 0); /* Create a socket */
	if (sockfd < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	memset(&server_addr, 0, sizeof(server_addr)); /* Prepare the sockaddr_in */
	server_addr.sin_family = AF_INET; /* IPv4 */
	server_addr.sin_addr.s_addr = INADDR_ANY; /* Any address */
	server_addr.sin_port = htons(PORT); /* Port number */
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Bind failed"); /* Bind the socket */
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	if (listen(sockfd, 1) < 0) /* Listen on the socket */
	{
		perror("Listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);
	newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
	if (newsockfd < 0)
	{
		perror("Accept failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	char client_ip[INET_ADDRSTRLEN]; /* Print the client's IP address */
	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
	printf("Client connected: %s\n", client_ip);
	/* Receive a message from the client */
	bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received < 0)
		perror("Receive failed");
	else
	{
		buffer[bytes_received] = '\0'; /* Null-terminate the received string */
		printf("Message received: \"%s\"\n", buffer);
	}
	close(newsockfd); /* Close the connection with the client */
	close(sockfd); /* Close the server socket */
	return (0);
}
