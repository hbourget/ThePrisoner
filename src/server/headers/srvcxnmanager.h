#include "../../common/config.h"

#ifndef SRVCXNMANAGER_H
#define SRVCXNMANAGER_H

#define BUFFERSIZE 2048
#define MAXSIMULTANEOUSCLIENTS 100

typedef struct {
    int sockfd;
    struct sockaddr address;
    int addr_len;
    int index;
} connection_t;


void init_sockets_array();
void add(connection_t *connection);
void del(connection_t *connection);
void *threadProcess(void *ptr);
int create_server_socket(ServerConfig cfgServer) ;

#endif /* SRVCXNMANAGER_H */