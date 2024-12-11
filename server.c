/*
* CPE 2600 121
* Lab 13 - Final Project
* Filename: server.c
* Description: Start the chatting program with the client, bi directional communication
* Date: 12/11/2024
* Compile instruction: make OR gcc server.c -o server
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
    int server_fd, client_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    //create a socket
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    //bind socket to a file
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    unlink(SOCKET_PATH);//remove any existing file
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("blind error");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //listen for a connection
    if (listen(server_fd, 5) == -1) {
        perror("listen error");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on %s\n", SOCKET_PATH);

    //Accept a connection
    if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
        perror("accept error");
        close(server_fd);
        unlink(SOCKET_PATH);
        exit(EXIT_FAILURE);
    } else {
        printf("connection accepted\n");
        sleep(3);
    }

    //Read data from the client and send a new message back
    while(1) {
        int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Client: %s\n", buffer);

            printf("You:");
            fgets(buffer, 99, stdin);
            write(client_fd, buffer, strlen(buffer));

        } else {
            printf("Client disconnected.\n");
            break;
        }
    }

    //clean up
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}