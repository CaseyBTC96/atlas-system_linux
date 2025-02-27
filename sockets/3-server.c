#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

/**
 * setup_server - Initializes the server socket.
 *
 * @server_addr: Pointer to a sockaddr_in structure that contains
 *                the server's address information.
 *
 * Return: The file descriptor for the server socket.
 */
int setup_server(struct sockaddr_in *server_addr)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(server_addr, 0, sizeof(*server_addr));
	server_addr->sin_family = AF_INET;
	server_addr->sin_addr.s_addr = INADDR_ANY;
	server_addr->sin_port = htons(PORT);

	if (bind(sockfd, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0)
	{
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 1) < 0)
	{
		perror("Listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	return (sockfd);
}

/**
 * handle_client - Accepts a client connection and handles message reception.
 *
 * @sockfd: The file descriptor for the server socket.
 */
void handle_client(int sockfd)
{
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	char buffer[BUFFER_SIZE], client_ip[INET_ADDRSTRLEN];
	ssize_t bytes_received;

	int newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

	if (newsockfd < 0)
	{
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}

	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
	printf("Client connected: %s\n", client_ip);

	bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received < 0)
		perror("Receive failed");
	else
	{
		buffer[bytes_received] = '\0';
		printf("Message received: \"%s\"\n", buffer);
	}

	close(newsockfd);
}

/**
 * main - Entry point of the server program.
 *
 * Return: 0 on success.
 */
int main(void)
{
	struct sockaddr_in server_addr;
	int sockfd = setup_server(&server_addr);

	printf("Server listening on port %d\n", PORT);

	handle_client(sockfd);

	close(sockfd);
	return (0);
}
