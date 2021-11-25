/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: aurelio
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <stdbool.h>

#include "../headers/srvcxnmanager.h"
#include "../headers/config.h"

int main(int argc, char** argv) 
{
    ServerConfig cfgServer = initCfg();

    showConfig(cfgServer);
    showRooms(cfgServer);

    int sockfd = -1;
    int index = 1;
    connection_t *connection;
    pthread_t thread;

    /* init array*/
    init_sockets_array();
    /* create socket */
    sockfd = create_server_socket(cfgServer);

    /* listen on port , stack size 50 for incoming connections*/
    if (listen(sockfd, 50) < 0) {
        fprintf(stderr, "\033[0;31m %s: Error: cannot listen on port\033[0m\n", argv[0]);
        return -5;
    }

    printf("\033[0;32m\n --> Server is ready and listening.\033[0m\n");

    //Wait for connection
    while (true) {
        /* accept incoming connections */
        connection = (connection_t *) malloc(sizeof (connection_t));
        connection->sockfd = accept(sockfd, &connection->address, &connection->addr_len);
        connection->index = index++;
        if (connection->sockfd <= 0) {
            free(connection);
        } else {
            /* start a new thread but do not wait for it */
            pthread_create(&thread, 0, threadProcess, (void *) connection);
            pthread_detach(thread);
        }
    }
    return (EXIT_SUCCESS);
}

