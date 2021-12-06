
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libconfig.h>

#include "../headers/srvcxnmanager.h"
#include "../headers/config.h"
#include "../headers/game.h"

connection_t* connections[MAXSIMULTANEOUSCLIENTS];

void init_sockets_array() {
    for (int i = 0; i < MAXSIMULTANEOUSCLIENTS; i++) {
        connections[i] = NULL;
    }
}
void add(connection_t *connection) {
    for (int i = 0; i < MAXSIMULTANEOUSCLIENTS; i++) {
        if (connections[i] == NULL) {
            connections[i] = connection;
            return;
        }
    }
    perror("Too much simultaneous connections");
    exit(-5);
}

void del(connection_t *connection) {
    for (int i = 0; i < MAXSIMULTANEOUSCLIENTS; i++) {
        if (connections[i] == connection) {
            connections[i] = NULL;
            return;
        }
    }
    perror("Connection not in pool ");
    exit(-5);
}

void *threadProcess(void *ptr) {
    ServerConfig cfgServer = initCfg();
    char buffer_in[BUFFERSIZE];
    char buffer_out[BUFFERSIZE];
    int len;
    connection_t *connection;

    /* Remplissage de la struct (test - à virer ensuite)*/
    PlayerGameSettings configuration;
    /*configuration.balance = 566;
    configuration.totalR = 7;*/

    if (!ptr) pthread_exit(0);
    connection = (connection_t *) ptr;
    printf("New incoming connection \n");

    add(connection);

    char *str = malloc(sizeof(strlen(buffer_in)));
    len = read(connection->sockfd, buffer_in, BUFFERSIZE);

    for(int i = 0; i < cfgServer.gameConfig.nbRooms; i++) {
        memset(str, 0, strlen(buffer_in));
        strcat(str, buffer_in);

        //Verifie si le joueur qui vient de se connecter est bien attribué à une room.
        if(strcmp(str, cfgServer.gameConfig.rooms[i].idClient_1) == 0 || strcmp(str, cfgServer.gameConfig.rooms[i].idClient_2) == 0) {
            configuration = initPlayerGameSettings(cfgServer, i);
            send(connection->sockfd, &configuration, sizeof(configuration), 0);
        }
    }

    while ((len = read(connection->sockfd, buffer_in, BUFFERSIZE)) > 0) {
        if (strncmp(buffer_in, "bye", 3) == 0) {
            break;
        }
        strcpy(buffer_out, "\n\033[1;37mServer Echo : \033[0m");
        strncat(buffer_out, buffer_in, len);

        write(connection->sockfd, buffer_out, strlen(buffer_out));

        //clear input buffer
        memset(buffer_in, '\0', BUFFERSIZE);
    }

    printf("Connection to client \033[0;36m#%i\033[0m has ended.\n", connection->index);
    close(connection->sockfd);
    del(connection);
    free(connection);
    pthread_exit(0);
}

int create_server_socket(ServerConfig cfgServer) {
    int sockfd = -1;
    struct sockaddr_in address;

    /* create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd <= 0) {
        fprintf(stderr, "\033[0;31m %s: Error: cannot create socket\033[0m\n");
        return -3;
    }


    /* bind socket to port */
    address.sin_family = AF_INET;
    //bind to all ip : 
    //address.sin_addr.s_addr = INADDR_ANY;
    //ou 0.0.0.0 
    //Sinon  127.0.0.1

    address.sin_addr.s_addr = inet_addr(cfgServer.serverIP);
    address.sin_port = htons(cfgServer.serverPort);

    /* prevent the 60 secs timeout */
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*) &reuse, sizeof (reuse));

    /* bind */
    if (bind(sockfd, (struct sockaddr *) &address, sizeof (struct sockaddr_in)) < 0) {
        fprintf(stderr, "\033[0;31mError: cannot bind socket to port %d\033[0m\n", cfgServer.serverPort);
        return -4;
    }

    return sockfd;
}
