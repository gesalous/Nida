#ifndef NIDA_H
#define NIDA_H
#define MAX_MESSAGE_SIZE 256

#define SOCKET_PATH "/tmp/nida.sock"

struct nida_message {
    int type;           // Message type
    char message[MAX_MESSAGE_SIZE];       // Message content
    char response[MAX_MESSAGE_SIZE+32];      // Response content
}__attribute__((packed));

#endif 

