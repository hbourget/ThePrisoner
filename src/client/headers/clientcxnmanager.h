#include "../../common/config.h"

#ifndef CLIENTCXNMANAGER_H
#define CLIENTCXNMANAGER_H

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

#endif /* CLIENTCXNMANAGER_H */

