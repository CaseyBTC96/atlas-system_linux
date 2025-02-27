#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define RESPONSE "HTTP/1.1 200 OK\r\n\r\n"

void parse_http_parameters(char *request);
char *make_response(char *address, char *request);

int main(void)
{
    int socket_fd, client_socket;
    size_t bytes = 0;
    char buffer[4096];
    struct sockaddr_in server_address;
    socklen_t addrlen = sizeof(server_address);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
    if (listen(socket_fd, 5) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        client_socket = accept(socket_fd, (struct sockaddr *)&server_address, &addrlen);
        if (client_socket < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        char client_address[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &server_address.sin_addr, client_address, sizeof(client_address));
        bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0)
        {
            buffer[bytes] = '\0';
            make_response(client_address, buffer);
        }

        send(client_socket, RESPONSE, strlen(RESPONSE), 0);
        close(client_socket);
    }

    close(socket_fd);
    return 0;
}

char *make_response(char *address, char *request)
{
    printf("Client connected: %s\n", address);
    printf("Raw request: \"%s\"\n", request);
    parse_http_parameters(request);
    fflush(stdout);
    return strdup(RESPONSE);
}

void parse_http_parameters(char *request)
{
    char *body_params = strstr(request, "\r\n\r\n") + 4;
    char *path = strtok(strtok(strchr(request, ' ') + 1, " "), "?");
    char *key, *value;

    printf("Path: %s\n", path);

    for (
        key = strtok(body_params, "="), value = strtok(NULL, "&");
        key && value;
        key = strtok(NULL, "="), value = strtok(NULL, "&")
    )
    {
        printf("Body param: \"%s\" -> \"%s\"\n", key, value);
    }
}
