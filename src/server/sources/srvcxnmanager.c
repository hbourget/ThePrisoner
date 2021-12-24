
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libconfig.h>

#include "../headers/srvcxnmanager.h"
#include "../../common/config.h"
#include "../../common/game.h"

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
    ServerConfig cfgServer = initServerCfg();
    ClientConfig cfgClient;
    PlayerGameSettings cfgPlayer;
    gameData.currentRound = 0;
    int len = 0;

    if (!ptr) pthread_exit(0);
    connection = (connection_t *) ptr;
    add(connection);

    //Attend que le client clique sur "Se connecter"
    while((len = read(connection->sockfd, &cfgClient, sizeof(cfgClient))) > 0)
    {
        printf("Client ID \033[0;36m#%d\033[0m has \033[0;32mconnected\033[0m.\n", cfgClient.idClient, connection->index);
        break;
    }

    for(int i = 0; i < cfgServer.gameConfig.nbRooms; i++) 
    {
        const char *roomName = cfgServer.gameConfig.rooms[i].name;
        //Verifie si le joueur qui vient de se connecter est bien attribué à une room.
        if(cfgClient.idClient == cfgServer.gameConfig.rooms[i].idClient_1 || cfgClient.idClient == cfgServer.gameConfig.rooms[i].idClient_2) 
        {
            //Initialisation de la configuration propre au client qui vient de se connecter.
            cfgPlayer = initPlayerGameSettings(cfgServer, i, cfgClient.idClient);
            send(connection->sockfd, &gameData, sizeof(gameData), 0);

            while((len = read(connection->sockfd, &cfgPlayer, sizeof(cfgPlayer))) > 0) 
            {
                gameData = hydrateGameData(cfgPlayer, gameData, cfgServer, i);

                //Le serveur bloque dans cette section et attend que deux clients de la même room se connecte.
                if(gameData.p1.idClient != 0 && gameData.p2.idClient != 0) 
                {
                    printf("(\033[0;33m%s\033[0m) Both client have connected.\n", roomName);
                    break;
                } 
                else 
                {
                    printf("(\033[0;33m%s\033[0m) Waiting for both clients...\n", roomName);
                }
            }
            // Une fois les 2 client de la même room connectés, on bloque dans la boucle de jeu ci dessous
            while((len = read(connection->sockfd, &cfgPlayer, sizeof(cfgPlayer))) > 0)
            {
                //A la reception d'un envoi du client, hydratation de gameData, notamment pour récupérer le status de réponse.
                gameData = hydrateGameData(cfgPlayer, gameData, cfgServer, i);

                //Si les deux joueurs on répondu, on joue le round
                //! MARCHE PAS, TROUVER UNE SOLUTION ! (Seulement le dernier client qui a "Validate" est mis à jour)
                if(gameData.p1.responded == true && gameData.p2.responded == true)
                {
                    gameData = playRound(gameData);
                    printf("(\033[0;33m%s\033[0m) Received data, playing the round %d/%d...\n", roomName, gameData.currentRound, gameData.totalRounds);
                    printf("Balance of P1: %d\n", gameData.p1.balance);
                    printf("Balance of P2: %d\n", gameData.p2.balance);
                    //Renvoi des structures aux bons clients
                    if(gameData.p1.idClient == cfgClient.idClient)
                    {
                        cfgPlayer = gameData.p1;
                        send(connection->sockfd, &cfgPlayer, sizeof(cfgPlayer), 0);
                    }
                    else
                    {
                        cfgPlayer = gameData.p2;
                        send(connection->sockfd, &cfgPlayer, sizeof(cfgPlayer), 0);
                    }
                    printf("(\033[0;33m%s\033[0m) Round %d/%d has successfully been played.\n", roomName, gameData.currentRound, gameData.totalRounds);
                }
                
                //Vérification si le nombre total de manches à été atteint, si oui, affichage du résultat
                if(isGameFinished(gameData))
                {
                    printf("(\033[0;33m%s\033[0m) Received data, game has finished. Drawing results...\n", roomName);
                    int idWinner = getWinner(gameData);
                    if(idWinner == 0)
                    {
                        printf("(\033[0;33m%s\033[0m) No winner, game is tied.\n", roomName);
                    }
                    else
                    {
                        printf("(\033[0;33m%s\033[0m) Winner is client #%d\n", roomName, idWinner);
                    }
                    //TODO Ecriture dans le csv : choix + temps de réponse (mise en place d'une clock ?)
                    break;
                }
            }
        }
    }

    printf("Client ID \033[0;36m#%d \033[0mhas \033[0;31mdisconnected\033[0m.\n", cfgClient.idClient);
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
        fprintf(stderr, "\033[0;31m Error: cannot create socket\033[0m\n");
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
