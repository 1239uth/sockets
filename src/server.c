//
// Created by Uthman Mohamed on 2023-09-22.
//

#include "../include/server.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

int main()
{
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int len;
    int fd, client_fd;
    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    bzero(buf, sizeof(buf));

    /* setup passive open */
    if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("server: socket");
        exit(1);
    }
    if ((bind(fd, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
        perror("server: bind");
        exit(1);
    }
    listen(fd, MAX_PENDING);

    printf("Server is listening on port %d\n", SERVER_PORT);

    /* wait for connection, then receive and print text */
    while(1) {
        if ((client_fd = accept(fd, (struct sockaddr *)&sin, &len)) < 0) {
            perror("server: accept");
            exit(1);
        }
        while (len = recv(client_fd, buf, sizeof(buf), 0)){

            /* Logic to strip the NULL chars */
            int i;
            for(i=0;i<sizeof(buf);i++) {
                if(buf[i] != '\0')
                    printf("%c", buf[i]);
            }

            /* No need to fputs, as we are already printing
               one char at a time */
            //fputs(buf, stdout);
            bzero(buf, sizeof(buf));
            fflush(stdout);
        }
        close(client_fd);
    }
}
