#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PORT 9000
#define BUFFER_SIZE 1024

int main() {
  int sock;
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE];

  sock = socket(AF_INET, SOCK_STREAM, 0);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  int is_connected =
      connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

  while (is_connected) {
    printf("> ");

    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
      break;

    send(sock, buffer, strlen(buffer), 0);

    memset(buffer, 0, BUFFER_SIZE);

    int bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);

    if (bytes <= 0)
      break;

    printf("Server: %s", buffer);
  }

  close(sock);
  return 0;
}
