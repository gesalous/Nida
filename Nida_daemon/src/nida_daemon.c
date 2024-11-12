#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "../include/nida.h"

static volatile uint64_t keep_running = 1;

static void nida_signal_handler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        keep_running = 0;
    }
}

static void nida_process_message(struct nida_message *msg) {
    printf("Received message:\n");
    printf("Type: %d\n", msg->type);
    printf("Message: %.*s\n", (int)sizeof(msg->message), msg->message);
    
    switch(msg->type) {
        case 1:
            snprintf(msg->response, sizeof(msg->response), 
                    "Processed type 1: %.*s", (int)sizeof(msg->message), msg->message);
            break;
        case 2:
            snprintf(msg->response, sizeof(msg->response), 
                    "Processed type 2: %.*s", (int)sizeof(msg->message), msg->message);
            break;
        default:
            snprintf(msg->response, sizeof(msg->response), 
                    "Unknown type %d", msg->type);
    }
}

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    struct nida_message msg;
    
    // Setup signal handling
    signal(SIGINT, nida_signal_handler);
    signal(SIGTERM, nida_signal_handler);
    
    // Create socket with SOCK_SEQPACKET
    server_fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }
    
    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    
    // Remove existing socket file
    unlink(SOCKET_PATH);
    
    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(1);
    }
    
    // Listen for connections
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        unlink(SOCKET_PATH);
        exit(1);
    }
    
    printf("Server started. Listening on %s\n", SOCKET_PATH);
    
    while (keep_running) {
        client_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_fd == -1) {
            if (errno == EINTR) continue;
            perror("accept");
            break;
        }
        
        // Read message - SOCK_SEQPACKET preserves message boundaries
        ssize_t bytes_read = recv(client_fd, &msg, sizeof(msg), 0);
        if (bytes_read == sizeof(msg)) {
            // Process and respond
            nida_process_message(&msg);
            send(client_fd, &msg, sizeof(msg), 0);
        }
        
        close(client_fd);
    }
    
    // Cleanup
    close(server_fd);
    unlink(SOCKET_PATH);
    printf("Server terminated.\n");
    
    return 0;
}

