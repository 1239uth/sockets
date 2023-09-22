//
// Created by Uthman Mohamed on 2023-09-22.
//

#include "../include/server.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

void print_all_chars(char *buf, unsigned long size);

char *get_date_and_time();


int main() {
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    char ret[MAX_LINE];
    int len;
    int fd, client_fd;
    /* build address data structure */
    bzero((char *) &sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    bzero(buf, sizeof(buf));

    /* setup passive open */
    if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("server: socket");
        exit(1);
    }
    if ((bind(fd, (struct sockaddr *) &sin, sizeof(sin))) < 0) {
        perror("server: bind");
        exit(1);
    }
    listen(fd, MAX_PENDING);

    printf("Server is listening on port %d\n", SERVER_PORT);

    /* wait for connection, then receive and print text */
    while (1) {
        if ((client_fd = accept(fd, (struct sockaddr *) &sin, &len)) < 0) {
            perror("server: accept");
            exit(1);
        }
        printf("Connected to %s successfully!\n", inet_ntoa(sin.sin_addr));
        do {
            len = recv(client_fd, buf, sizeof(buf), 0);

            printf("Received message: ");
            print_all_chars(buf, sizeof(buf));

            char *date_and_time = get_date_and_time();

            sprintf(ret, "On %s you said: %s", date_and_time, buf);

            // send message to client
            send(client_fd, ret, sizeof(ret), 0);

            bzero(ret, sizeof(ret));
            bzero(buf, sizeof(buf));
            fflush(stdout);
        } while (len);
        close(client_fd);
    }
}

void print_all_chars(char *buf, unsigned long size) {
    for (int i = 0; i < size; i++) {
        if (buf[i] != '\0')
            printf("%c", buf[i]);
    }
}

char *get_date_and_time() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char *s = asctime(tm);
    s[strlen(s) - 1] = '\0';
    return s;
}