#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <libconfig.h>

#include "../headers/clientcxnmanager.h"
#include "../../common/config.h"
#include "../../common/game.h"

PlayerGameSettings pgm;

void *threadProcess(void * ptr) {
    PlayerGameSettings cfgPlayer;
    ClientConfig cfgClient = initClientCfg();
    int sockfd = *((int *) ptr);

    //Envoi de la configuration client au serveur
    write(sockfd, &cfgClient, sizeof(cfgClient));
    //Lecture de la configuration initiale du joueur
    read(sockfd, &cfgPlayer, sizeof(cfgPlayer));
    printf("%d", cfgPlayer.idClient);

    //cfgPlayer.bet = 200;
    //write(sockfd, &cfgPlayer, sizeof(cfgPlayer));

    //while ((len = read(sockfd, &cfgPlayer, sizeof(cfgPlayer))) > 0);

    //close(sockfd);
    //printf("client pthread ended, len=%d\n", len);
}

int open_connection(ClientConfig cfgClient) {
    int sockfd;
    struct sockaddr_in serverAddr;

    // Create the socket. 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Configure settings of the server address
    // Address family is Internet 
    serverAddr.sin_family = AF_INET;
    //Set port number, using htons function 
    serverAddr.sin_port = htons(cfgClient.serverPort);
    //Set IP address to localhost
    serverAddr.sin_addr.s_addr = inet_addr(cfgClient.serverIP);

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    //Connect the socket to the server using the address
    if (connect(sockfd, (struct sockaddr *) &serverAddr, sizeof (serverAddr)) != 0) {
        printf("Fail to connect to server");
        exit(-1);
    };

    return sockfd;
}
