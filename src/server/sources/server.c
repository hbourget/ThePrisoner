
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libconfig.h>

#include "../headers/server.h"
#include "../../common/configurations.h"

connection_t* connections[MAXSIMULTANEOUSCLIENTS];
ServerConfig cfgServer;
int counter = 0;
int result = -1;


void setCfgServer(ServerConfig cfg){
    cfgServer = cfg;
}
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
    ClientConfig cfgClient;
    PlayerGameSettings cfgPlayer;
    int len = 0;
    FILE *file;

    //Ecriture de l'entête du fichier results.csv (qu'une seule fois)
    if(counter == 0)
    {
        writeHeader(file);
    }
    counter++;

    if (!ptr) pthread_exit(0);
    connection = (connection_t *) ptr;
    add(connection);

    //Attend que le client clique sur "Se connecter"
    while((len = read(connection->sockfd, &cfgClient, sizeof(cfgClient))) > 0)
    {
        printf("Client \033[0;36m#%d\033[0m has \033[0;32mconnected\033[0m.\n", cfgClient.idClient, connection->index);
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
            send(connection->sockfd, &cfgPlayer, sizeof(cfgPlayer), 0);

            while((len = read(connection->sockfd, &cfgPlayer, sizeof(cfgPlayer))) > 0) 
            {
                cfgServer.gameConfig.rooms[i].gameData = firstHydrateGameData(cfgPlayer, cfgServer.gameConfig.rooms[i].gameData, cfgServer, i);

                //Le serveur bloque dans cette section et attend que deux clients de la même room se connecte.
                if(cfgServer.gameConfig.rooms[i].gameData.p1.idClient != 0 && cfgServer.gameConfig.rooms[i].gameData.p2.idClient  != 0) 
                {
                    printf("(\033[0;33mRoom %s\033[0m) Both client have connected.\n", roomName);
                    break;
                } 
                else 
                {
                    printf("(\033[0;33mRoom %s\033[0m) Waiting for both clients...\n", roomName);
                }
            }
            // Une fois les 2 client de la même room connectés, on bloque dans la boucle de jeu ci dessous
            while((len = read(connection->sockfd, &cfgPlayer, sizeof(cfgPlayer))) > 0)
            {
                //A la reception d'un envoi du client, hydratation de gameData, notamment pour récupérer le status de réponse.
                cfgServer.gameConfig.rooms[i].gameData = hydrateGameData(cfgPlayer, cfgServer.gameConfig.rooms[i].gameData, cfgServer, i);

                //Si les deux joueurs on répondu, on joue le round
                if(cfgServer.gameConfig.rooms[i].gameData.p1.responded == true && cfgServer.gameConfig.rooms[i].gameData.p2.responded == true)
                {
                    cfgServer.gameConfig.rooms[i].gameData = playRound(cfgServer.gameConfig.rooms[i].gameData);
                    printf("(\033[0;33mRoom %s\033[0m) Playing round %d/%d...\n", roomName, cfgServer.gameConfig.rooms[i].gameData.currentRound, cfgServer.gameConfig.rooms[i].gameData.totalRounds);
                    printf("(\033[0;33mRoom %s\033[0m) P1 action: %d (1=BETRAY, 2=COOP)\n", roomName, cfgServer.gameConfig.rooms[i].gameData.p1.action);
                    printf("(\033[0;33mRoom %s\033[0m) P2 action: %d (1=BETRAY, 2=COOP)\n", roomName, cfgServer.gameConfig.rooms[i].gameData.p2.action);
                    printf("(\033[0;33mRoom %s\033[0m) P1 bet: %d\n", roomName, cfgServer.gameConfig.rooms[i].gameData.p1.bet);
                    printf("(\033[0;33mRoom %s\033[0m) P2 bet: %d\n", roomName, cfgServer.gameConfig.rooms[i].gameData.p2.bet);
                    printf("\n");
                    printf("(\033[0;33mRoom %s\033[0m) Balance of P1: %d\n", roomName, cfgServer.gameConfig.rooms[i].gameData.bal_p1);
                    printf("(\033[0;33mRoom %s\033[0m) Balance of P2: %d\n", roomName, cfgServer.gameConfig.rooms[i].gameData.bal_p2);
                    writeResults(file, roomName, cfgServer.gameConfig.rooms[i].gameData);
                    printf("(\033[0;33mRoom %s\033[0m) Round results have been written in results.csv\n", roomName);
                    printf("(\033[0;33mRoom %s\033[0m) Round %d/%d has successfully been played.\n\n", roomName, cfgServer.gameConfig.rooms[i].gameData.currentRound, cfgServer.gameConfig.rooms[i].gameData.totalRounds);
                }
                
                //Vérification si le nombre total de manches à été atteint, si oui, affichage du résultat
                if(isGameFinished(cfgServer.gameConfig.rooms[i].gameData))
                {
                    printf("(\033[0;33mRoom %s\033[0m) Game has finished. Drawing results...\n", roomName);
                    int idWinner = getWinner(cfgServer.gameConfig.rooms[i].gameData);
                    if(idWinner == 0)
                    {
                        printf("(\033[0;33mRoom %s\033[0m) Game result : \033[1;35mTIE\033[0m).\n", roomName);
                        result = 3;
                        write(connection->sockfd, &result, sizeof(result));
                    }
                    else
                    {
                        printf("(\033[0;33mRoom %s\033[0m) Game result : \033[1;32m#%d won\033[0m.\n", roomName, idWinner);
                        if(idWinner == cfgPlayer.idClient){
                            result = 1;
                            write(connection->sockfd, &result, sizeof(result));
                        }else{
                            result = 2;
                            write(connection->sockfd, &result, sizeof(result));
                        }
                    }
                    break;
                }
            }
            GameData resetGameData;
            cfgServer.gameConfig.rooms[i].gameData = resetGameData;
        }
    }

    printf("Client \033[0;36m#%d \033[0mhas \033[0;31mdisconnected\033[0m.\n", cfgClient.idClient);
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
