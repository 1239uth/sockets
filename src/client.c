//
// Created by Uthman Mohamed on 2023-09-22.
//

#include "../include/client.h"
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define PORT 5432
#define MAX_LINE 256

/**
 * Client application that connects to server.c by a socket connection and sends a message.
 */
int main(int argc, char *argv[]) {
    struct hostent *host_pointer;
    struct sockaddr_in sin;
    char *host;
    char buf[MAX_LINE];
    int server_fd;
    if (argc == 2) {
        host = argv[1];
    } else {
        fprintf(stderr, "usage: %s <host>\n", argv[0]);
        exit(1);
    }

    /* translate host name into peer.server_fd IP address */
    host_pointer = gethostbyname(host);
    if (!host_pointer) {
        fprintf(stderr, "%s: unknown host: %server_fd\n", argv[0], host);
        exit(1);
    }
    /* build address data structure */
    bzero((char *) &sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(host_pointer->h_addr, (char *) &sin.sin_addr, host_pointer->h_length);
    sin.sin_port = htons(PORT);
    /* active open */
    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("client: socket");
        exit(1);
    }
    if (connect(server_fd, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("client: connect");
        close(server_fd);
        exit(1);
    }

    printf("Connection established!\n");

    /* main loop: get and send lines of text */
    char *ret;
    do {
        printf("\x1b[33mEnter a message: \x1b[0m");
        ret = fgets(buf, sizeof(buf), stdin);

        buf[MAX_LINE - 1] = '\0';
        send(server_fd, buf, strlen(buf) + 1, 0);

        // receive message from server
        recv(server_fd, buf, sizeof(buf), 0);

        printf("\x1b[32mServer responded: \x1b[0m%s\n", buf);

    } while (ret);

    return 0;
}