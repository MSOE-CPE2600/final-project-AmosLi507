/*
* CPE 2600 121
* Lab 13 - Final Project
* Filename: client.c
* Description: Start the chatting program with the client, bi directional communication
* Date: 12/11/2024
* Compile instruction: make  OR gcc client.c -o client
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "tmp/my_socket"
#define BUFFER_SIZE 100

int main() {
    int client_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE] = "Hello message from client!";

    //create socket
    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    //set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    //connect to the server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect error");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    //Read data from the client and send a new message back
    while(1) {
        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(client_fd, buffer, strlen(buffer));

        int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Server: %s\n", buffer);
        } else {
            printf("Server disconnected.\n");
            break;
        }
    }

    close(client_fd);
    return 0;
}