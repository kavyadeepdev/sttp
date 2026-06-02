#include "../include/constants.h"
#include "../include/crypto.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len;
	char buffer[BUFFER_SIZE];
	time_t server_start = time(NULL);

	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Failed to bind to port %d\n", PORT);
		return 1;
	}

	listen(server_fd, 5);
	printf("Server listening on port %d\n", PORT);

	while (1) {
		client_len = sizeof(client_addr);
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
		printf("Client connected.\n");

		while (1) {
			memset(buffer, 0, BUFFER_SIZE);
			int bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

			if (bytes <= 0)
				break;

			crypto_caesar_cipher(buffer, 26 - SHIFT);
			buffer[strcspn(buffer, "\r\n")] = 0;

			if (strcmp(buffer, "PING") == 0) {
				crypto_send_encrypted(client_fd, "PONG\n");

			} else if (strcmp(buffer, "HELP") == 0) {
				crypto_send_encrypted(client_fd,
									  "PING HELP VERSION IDENTIFY TIME UPTIME STATUS ECHO INFO DISCONNECT EXIT\n");

			} else if (strcmp(buffer, "VERSION") == 0) {
				crypto_send_encrypted(client_fd, "STTP/1.0\n");

			} else if (strcmp(buffer, "IDENTIFY") == 0) {
				crypto_send_encrypted(client_fd, "LOCAL_SERVER\n");

			} else if (strcmp(buffer, "TIME") == 0) {
				time_t now = time(NULL);
				struct tm *t = localtime(&now);
				char response[64];
				strftime(response, sizeof(response), "%H:%M:%S\n", t);
				crypto_send_encrypted(client_fd, response);

			} else if (strcmp(buffer, "UPTIME") == 0) {
				long uptime = time(NULL) - server_start;
				char response[64];
				snprintf(response, sizeof(response), "%ld seconds\n", uptime);
				crypto_send_encrypted(client_fd, response);

			} else if (strcmp(buffer, "STATUS") == 0) {
				crypto_send_encrypted(client_fd, "OK\n");

			} else if (strncmp(buffer, "ECHO ", 5) == 0) {
				char response[BUFFER_SIZE];
				snprintf(response, sizeof(response), "%s\n", buffer + 5);
				crypto_send_encrypted(client_fd, response);

			} else if (strcmp(buffer, "INFO") == 0) {
				crypto_send_encrypted(client_fd, "Simple Text Transfer Protocol (STTP) server\n");

			} else if (strcmp(buffer, "EXIT") == 0) {
				crypto_send_encrypted(client_fd, "CONNECTION_CLOSED\n");
				break;

			} else {
				crypto_send_encrypted(client_fd, "UNKNOWN_COMMAND\n");
			}
		}

		close(client_fd);
		printf("Client disconnected.\n");
	}

	close(server_fd);
	return 0;
}
