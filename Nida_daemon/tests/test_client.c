#include "../include/nida.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int sock_fd;
  struct sockaddr_un server_addr;
  struct nida_message msg;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <type> <message>\n", argv[0]);
    exit(1);
  }

  // Create socket with SOCK_SEQPACKET
  sock_fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (sock_fd == -1) {
    perror("socket");
    exit(1);
  }

  // Setup server address
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

  // Connect to server
  if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("connect");
    close(sock_fd);
    exit(1);
  }

  // Prepare message
  msg.type = atoi(argv[1]);
  strncpy(msg.message, argv[2], sizeof(msg.message) - 1);
  msg.message[sizeof(msg.message) - 1] = '\0';

  // Send message
  if (send(sock_fd, &msg, sizeof(msg), 0) == -1) {
    perror("send");
    close(sock_fd);
    exit(1);
  }

  // Read response - SOCK_SEQPACKET preserves message boundaries
  if (recv(sock_fd, &msg, sizeof(msg), 0) == -1) {
    perror("recv");
    close(sock_fd);
    exit(1);
  }

  printf("Server response: %s\n", msg.response);

  close(sock_fd);
  return 0;
}
