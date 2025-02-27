#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RESPONSE_OK "HTTP/1.1 200 OK\r\n\r\n"
#define RESPONSE_CREATED "HTTP/1.1 201 Created\r\nContent-Type: application/json\r\n\r\n"
#define RESPONSE_LENGTH_REQUIRED "HTTP/1.1 411 Length Required\r\n\r\n"
#define RESPONSE_UNPROCESSABLE_ENTITY "HTTP/1.1 422 Unprocessable Entity\r\n\r\n"
#define RESPONSE_NOT_FOUND "HTTP/1.1 404 Not Found\r\n\r\n"

typedef struct {
	int id;
	char title[100];
	char description[256];
} Todo;

Todo todos[100];
int todo_count = 0;

void parse_body(char *body, char *title, char *description) {
	char *title_param = strstr(body, "title=");
	char *description_param = strstr(body, "description=");
	
	if (title_param) {
		sscanf(title_param + 6, "%[^&]", title);
	}
	if (description_param) {
		sscanf(description_param + 12, "%[^&]", description);
	}
}

int create_todo(char *title, char *description) {
	if (todo_count < 100) {
		todos[todo_count].id = todo_count;
		strncpy(todos[todo_count].title, title, sizeof(todos[todo_count].title) - 1);
		strncpy(todos[todo_count].description, description, sizeof(todos[todo_count].description) - 1);
		return todo_count++;
	}
	return -1;
}

int main(void) {
	int socket_fd, new_con;
	size_t bytes = 0;
	char buffer[4096], meth[50], path[50], ver[50];
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	char response[512];

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

			if (strcmp(meth, "POST") == 0 && strcmp(path, "/todos") == 0) {
				char title[100] = {0}, description[256] = {0};
				char *content_length_header = strstr(buffer, "Content-Length: ");
				if (content_length_header) {
					int content_length = atoi(content_length_header + 16);
					char *body = buffer + bytes - content_length;
					*body = '\0'; /* Null-terminate the request */
					parse_body(body + 2, title, description); /* Skip the CRLF before the body */

					if (strlen(title) > 0 && strlen(description) > 0) {
						int id = create_todo(title, description);
						if (id != -1) {
							snprintf(response, sizeof(response), 
									RESPONSE_CREATED "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}", 
									id, title, description);
							send(new_con, response, strlen(response), 0);
						}
					} else {
						send(new_con, RESPONSE_UNPROCESSABLE_ENTITY, sizeof(RESPONSE_UNPROCESSABLE_ENTITY) - 1, 0);
					}
				} else {
					send(new_con, RESPONSE_LENGTH_REQUIRED, sizeof(RESPONSE_LENGTH_REQUIRED) - 1, 0);
				}
			} else {
				send(new_con, RESPONSE_NOT_FOUND, sizeof(RESPONSE_NOT_FOUND) - 1, 0);
			}
		}

		close(new_con);
	}

	close(socket_fd);
	return (0);
}
