#include "../include/constants.h"
#include "../include/crypto.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
	int sock = -1;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	while (1) {
		printf("> ");

		if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
			break;

		buffer[strcspn(buffer, "\n")] = 0;

		if (strcmp(buffer, "CONNECT") == 0) {
			if (sock != -1) {
				printf("Already connected.\n");
				continue;
			}

			sock = socket(AF_INET, SOCK_STREAM, 0);
			server_addr.sin_family = AF_INET;
			server_addr.sin_port = htons(PORT);
			server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

			if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
				printf("Connected to server!\n");
			} else {
				printf("Connection failed. Is the server running?\n");
				close(sock);
				sock = -1;
			}
			continue;
		}

		if (strcmp(buffer, "DISCONNECT") == 0) {
			if (sock == -1) {
				printf("Not connected.\n");
				continue;
			}

			crypto_send_encrypted(sock, "EXIT\n");

			memset(buffer, 0, BUFFER_SIZE);
			recv(sock, buffer, BUFFER_SIZE - 1, 0);
			crypto_caesar_cipher(buffer, 26 - SHIFT);
			printf("Server: %s", buffer);

			close(sock);
			sock = -1;
			printf("Disconnected.\n");
			continue;
		}

		if (strcmp(buffer, "EXIT") == 0) {
			if (sock != -1) {
				crypto_send_encrypted(sock, "EXIT\n");
				close(sock);
				sock = -1;
			}
			break;
		}

		if (sock == -1) {
			printf("Not connected. Type CONNECT first.\n");
			continue;
		}

		strcat(buffer, "\n");
		crypto_send_encrypted(sock, buffer);

		memset(buffer, 0, BUFFER_SIZE);
		int bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);

		if (bytes <= 0) {
			printf("Server disconnected.\n");
			close(sock);
			sock = -1;
			continue;
		}

		crypto_caesar_cipher(buffer, 26 - SHIFT);
		printf("Server: %s", buffer);

		if (strcmp(buffer, "CONNECTION_CLOSED\n") == 0) {
			close(sock);
			sock = -1;
			printf("Disconnected.\n");
		}
	}

	if (sock != -1)
		close(sock);

	return 0;
}
