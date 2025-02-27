#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RESPONSE_OK "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n"
#define RESPONSE_NOT_FOUND "HTTP/1.1 404 Not Found\r\n\r\n"

typedef struct {
	int id;
	char title[100];
	char description[256];
} Todo;

Todo todos[100];
int todo_count = 0;

void send_todos(int conn) {
	char response[2048] = {0};
	snprintf(response, sizeof(response), RESPONSE_OK);
	
	strcat(response, "[");
	for (int i = 0; i < todo_count; i++) {
		if (i > 0) strcat(response, ",");
		strcat(response, "{\"id\":");
		char id[4];
		snprintf(id, sizeof(id), "%d", todos[i].id);
		strcat(response, id);
		strcat(response, ",\"title\":\"");
		strcat(response, todos[i].title);
		strcat(response, "\",\"description\":\"");
		strcat(response, todos[i].description);
		strcat(response, "\"}");
	}
	strcat(response, "]");
	
	send(conn, response, strlen(response), 0);
}

int main(void) {
	int socket_fd, new_con;
	size_t bytes = 0;
	char buffer[4096], meth[50], path[50], ver[50];
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");
	fflush(stdout);

	if (listen(socket_fd, 5) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	while (1) {
		new_con = accept(socket_fd, (struct sockaddr *)&address, &addrlen);
		if (new_con < 0) {
			perror("accept failed");
			exit(EXIT_FAILURE);
		}

		printf("Client connected: %s\n", inet_ntoa(address.sin_addr));
		fflush(stdout);

		bytes = recv(new_con, buffer, sizeof(buffer) - 1, 0);
		if (bytes > 0) {
			buffer[bytes] = '\0';
			printf("Raw request: \"%s\"\n", buffer);
			fflush(stdout);
			sscanf(buffer, "%s %s %s", meth, path, ver);
			printf("Method: %s\nPath: %s\nVersion: %s\n", meth, path, ver);
			fflush(stdout);

			if (strcmp(meth, "GET") == 0 && strcmp(path, "/todos") == 0) {
				send_todos(new_con);
			} else {
				send(new_con, RESPONSE_NOT_FOUND, sizeof(RESPONSE_NOT_FOUND) - 1, 0);
			}
		}

		close(new_con);
	}

	close(socket_fd);
	return (0);
}
