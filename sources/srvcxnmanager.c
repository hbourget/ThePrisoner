
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
GameData gameData;


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
    connection_t *connection;
    ServerConfig cfgServer = initCfg();
    ClientConfig cfgClient;
    PlayerGameSettings cfgPlayer;
    int len = 0;

    if (!ptr) pthread_exit(0);
    connection = (connection_t *) ptr;
    add(connection);

    read(connection->sockfd, &cfgClient, sizeof(cfgClient));
    printf("Client \033[0;36m#%d\033[0m, is the client number \033[1;37m%i\033[0m to connect.\033[0m\n", cfgClient.idClient, connection->index);

    for(int i = 0; i < cfgServer.gameConfig.nbRooms; i++) 
    {
        //Verifie si le joueur qui vient de se connecter est bien attribué à une room.
        if(cfgClient.idClient == cfgServer.gameConfig.rooms[i].idClient_1 || cfgClient.idClient == cfgServer.gameConfig.rooms[i].idClient_2)
        {
            //Ecoute de ce qu'envoi le serveur
            while((len = read(connection->sockfd, &gameData, sizeof(gameData))) > 0)
            {
                //Initialisation et envoi de la configuration initiale au joueur
                cfgPlayer = initPlayerGameSettings(cfgServer, i, cfgClient.idClient);
                gameData = hydrateGameData(cfgPlayer, gameData, cfgServer, i);
                send(connection->sockfd, &gameData, sizeof(gameData), 0);

                if(gameData.p1.idClient != 0 && gameData.p2.idClient != 0)
                {
                    printf("Both client belonging to room %s have connected.\n", cfgServer.gameConfig.rooms[i].name);
                    //Server is now ready to send game infos to client and is listening to clients sending game infos in their room.
                }
                else
                {
                    printf("waiting for both clients...\n");
                }
            }
        }
    }


    /*len = 0; //check l'uiltité réel.
    while ((len = read(connection->sockfd, buffer_in, BUFFERSIZE)) > 0) {
        if (strncmp(buffer_in, "bye", 3) == 0) {
            break;
        }

        #if DEBUG
            printf("\033[1;32m----------------------------DEBUG----------------------------\033[0m\n");
            printf("\033[1;37mLen : \033[0;32m%i\033[0m\n", len);
            printf("\033[1;37mBuffer : \033[0;32m%.*s\033[0m", len, buffer_in);
            printf("\033[1;32m-------------------------------------------------------------\033[0m\n");
        #endif

        strcpy(buffer_out, "\n\033[1;37mServer Echo : \033[0m");
        strncat(buffer_out, buffer_in, len);

        write(connection->sockfd, buffer_out, strlen(buffer_out));

        //Reset du buffer d'entrée
        memset(buffer_in, '\0', BUFFERSIZE);
    }*/

    printf("Connection to client \033[0;36m#%d\033[0m has ended.\n", cfgClient.idClient);
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
