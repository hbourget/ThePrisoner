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

InterfaceParams interfaceParams;

/**
* @brief Affectation des valeurs à la structure InterfaceParams
*
* @param sockfd Socket générer grâce au donner du fichier client.config
* @param cfgClient Configuration client
*/
void setInterfaceParams(int socket, ClientConfig config){
    interfaceParams.sockfd = socket;
    interfaceParams.cfgClient = config;
}

/**
* @brief Récupération de l'attribut cfgClient de la structure InterfaceParams
*/
ClientConfig getClientConfig(){
    return interfaceParams.cfgClient;
}

/**
* @brief Récupération de l'attribut sockfd de la structure InterfaceParams
*/
int getClientSockfd(){
    return interfaceParams.sockfd;
}

void *threadProcess(void * ptr) {
    PlayerGameSettings cfgPlayer;
    int sockfd = *((int *) ptr);

    //Lecture de la configuration initiale du joueur
    read(sockfd, &cfgPlayer, sizeof(cfgPlayer));
    printf("%d", cfgPlayer.idClient);

    //cfgPlayer.bet = 200;
    //write(sockfd, &cfgPlayer, sizeof(cfgPlayer));

    //while ((len = read(sockfd, &cfgPlayer, sizeof(cfgPlayer))) > 0);

    //close(sockfd);
    //printf("client pthread ended, len=%d\n", len);
}

/**
* @brief Ouverture de la connexion vers le serveur
*
* @param cfgClient Configuration client (nécessaire pour récupérer le port et l'ip sur laquelle se connecter)
*/
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
