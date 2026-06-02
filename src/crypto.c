#include "../include/constants.h"
#include "../include/crypto.h"
#include <string.h>
#include <sys/socket.h>

void crypto_caesar_cipher(char *text, int shift) {
	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] >= 'a' && text[i] <= 'z') {
			text[i] = 'a' + (text[i] - 'a' + shift) % 26;
		} else if (text[i] >= 'A' && text[i] <= 'Z') {
			text[i] = 'A' + (text[i] - 'A' + shift) % 26;
		}
	}
}

void crypto_send_encrypted(int fd, const char *msg) {
	char buffer[BUFFER_SIZE];
	strncpy(buffer, msg, BUFFER_SIZE - 1);
	buffer[BUFFER_SIZE - 1] = '\0';
	crypto_caesar_cipher(buffer, SHIFT);
	send(fd, buffer, strlen(buffer), 0);
}
