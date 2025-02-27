#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define HTTP_RESPONSE "HTTP/1.1 200 OK\r\n\r\n"

void parse_http_headers(char *request);

int main(void)
{
	int server_socket, client_socket;
	size_t received_bytes = 0;
	char request_buffer[4096], response_message[] = HTTP_RESPONSE;
	struct sockaddr_in server_address;
	socklen_t address_length = sizeof(server_address);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
		perror("socket failed"), exit(EXIT_FAILURE);
	
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
		perror("bind failed"), exit(EXIT_FAILURE);

	printf("Server listening on port 8080\n");
	if (listen(server_socket, 5) < 0)
		perror("listen failed"), exit(EXIT_FAILURE);

	while (1)
	{
		client_socket = accept(server_socket, (struct sockaddr *)&server_address, &address_length);
		if (client_socket < 0)
			perror("accept failed"), exit(EXIT_FAILURE);
		
		printf("Client connected: %s\n", inet_ntoa(server_address.sin_addr));
		
		received_bytes = recv(client_socket, request_buffer, sizeof(request_buffer) - 1, 0);
		if (received_bytes > 0)
		{
			request_buffer[received_bytes] = '\0';
			printf("Raw request: \"%s\"\n", request_buffer), fflush(stdout);
			parse_http_headers(request_buffer);
		}
		
		send(client_socket, response_message, strlen(response_message), 0);
		close(client_socket);
	}
	return (0);
}

void parse_http_headers(char *request)
{
	char *header_line;
	char *header_array[16] = {0};
	char header_key[50], header_value[50];
	int i, header_count = 0;

	while ((header_line = strsep(&request, "\r\n")) != NULL)
	{
		if (header_count < 16)
			header_array[header_count++] = header_line;
	}

	for (i = 1; i < header_count; i++)
	{
		if (sscanf(header_array[i], "%[^:]: %[^\r\n]", header_key, header_value) == 2)
			printf("Header: \"%s\" -> \"%s\"\n", header_key, header_value), fflush(stdout);
	}
}
