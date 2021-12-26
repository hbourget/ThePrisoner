#include "../../common/configurations.h"

#ifndef CLIENT_H
#define CLIENT_H

#define BUFFERSIZE 2048

/**
* Structure permettant la transmission du socket et de la configuration client à l'interface graphique
*
* @param sockfd Socket générer grâce au donner du fichier client.config
* @param cfgClient Configuration client
*/
typedef struct {
    int sockfd;
    ClientConfig cfgClient;
} InterfaceParams;

void setInterfaceParams(int socket, ClientConfig config);
ClientConfig getClientConfig();
int getClientSockfd();
int open_connection(ClientConfig cfgClient);
void *threadProcess(void * ptr);
void setCfgPlayer(PlayerGameSettings cfg);
PlayerGameSettings getCfgPlayer();

#endif /* CLIENT_H */

