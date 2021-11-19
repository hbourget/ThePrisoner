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
#include "../headers/configurationHandler.h"

int main(int argc, char** argv) 
{
    configuration config;
    config = getConfig();
    char *showNb = (char*)malloc(sizeof(char));
    char *coop = (char*)malloc(sizeof(char));

    if(config.game.showNbRound == 1)
    {
        showNb = "Oui";
    }
    else
    {
        showNb = "Non";
    }

    if(config.game.coopMutuelle == 1)
    {
        coop = "50/50";
    }
    else
    {
        coop = "Proportionnel";
    }

    printf("\n --------------------------\nCONFIGURATION SERVEUR\n \n- Adresse IP: %s \n- Port: %d \n- Nombre de tours: %d \n- Temps par tour: %d \n- Solde joueur A: %d \n- Solde joueur B: %d \n- Montrer le nombre de tours: %s \n- Coopération mutuelle: %s \n--------------------------\n", config.sys.adresseIP, config.sys.port, config.game.nbRound, config.game.roundTime, config.game.bankA, config.game.bankB, showNb, coop);

    int sockfd = -1;
    int index = 1;
    connection_t *connection;
    pthread_t thread;

    /* init array*/
    init_sockets_array();
    /* create socket */
    sockfd = create_server_socket();

    /* listen on port , stack size 50 for incoming connections*/
    if (listen(sockfd, 50) < 0) {
        fprintf(stderr, "%s: error: cannot listen on port\n", argv[0]);
        return -5;
    }

    printf("ready and listening\n");

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

