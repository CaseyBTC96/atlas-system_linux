#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MESSAGE "HTTP/1.1 200 OK\r\n\r\n"

/**
* query_parser - Parses an HTTP query
* @query: Query string to parse
*/
void query_parser(char *query);

/**
* main - Entry point of the server
* Return: 0 on success
*/
int main(void)
{
	int socket_fd, client_fd;
	size_t bytes = 0;
	char buffer[4096], method[50], path[50], query[255], response[32] = MESSAGE;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");
	fflush(stdout);

	if (listen(socket_fd, 5) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		client_fd = accept(socket_fd, (struct sockaddr *)&address, &addrlen);
		if (client_fd < 0)
		{
			perror("accept failed");
			exit(EXIT_FAILURE);
		}

		printf("Client connected: %s\n", inet_ntoa(address.sin_addr));
		bytes = recv(client_fd, buffer, sizeof(buffer), 0);
		if (bytes > 0)
		{
			buffer[bytes] = '\0'; /* Null-terminate the buffer */
			printf("Raw request: \"%s\"\n", buffer);
			fflush(stdout);
			sscanf(buffer, "%s %[^?]?%s", method, path, query);
			printf("Path: %s\n", path);
			fflush(stdout);
			query_parser(query);
		}

		send(client_fd, response, sizeof(response) - 1, 0);
		close(client_fd);
	}

	close(socket_fd);
	return 0;
}

/**
* query_parser - Parses an HTTP query
* @query: Query string to parse
*/
void query_parser(char *query)
{
	int i = 0, flag = 0;
	char *token = NULL, *key_vals[16] = {0}, key[50], val[50];

	do
	{
		token = strsep(&query, "&");
		if (token && token[0])
		{
			key_vals[i++] = token;
			flag = 1;
		}
	} while (token && flag--);

	for (i = 0; key_vals[i]; i++)
	{
		sscanf(key_vals[i], "%[^=]=%s", key, val);
		printf("Query: \"%s\" -> \"%s\"\n", key, val);
		fflush(stdout);
	}
}
