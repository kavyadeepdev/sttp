#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PORT 9000
#define BUFFER_SIZE 1024

int main() {
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len = sizeof(client_addr);

	char buffer[BUFFER_SIZE];

	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	int has_binded =
		bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	if (has_binded < 0) {
		printf("bind failed\n");
		return 1;
	}

	listen(server_fd, 5);

	printf("Server listening on port %d\n", PORT);

	while (1) {
		client_fd =
			accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

		while (1) {
			memset(buffer, 0, BUFFER_SIZE);

			int bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

			if (bytes <= 0)
				break;

			buffer[strcspn(buffer, "\r\n")] = 0;

			if (strcmp(buffer, "PING") == 0) {
				send(client_fd, "PONG\n", 5, 0);
			} else {
				send(client_fd, "ERROR\n", 6, 0);
			}
		}

		close(client_fd);
	}

	close(server_fd);
	return 0;
}
